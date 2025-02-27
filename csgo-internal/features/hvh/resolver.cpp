#include "../../game/players.hpp"
#include "../../game/override_entity_list.hpp"
#include "../../globals.hpp"
#include "../../interfaces.hpp"
#include <algorithm>

using namespace sdk;

// Определяем константы
//constexpr float AIR_SPEED_THRESHOLD = 30.0f;
//constexpr int MAX_SWITCH_COUNT = 5;			
//constexpr float JITTER_BEGIN_ANGLE = 15.0f;	 
//constexpr int JITTER_CACHE_SIZE = 3;		

float resolver_t::get_angle(cs_player_t* player) {
	return math::normalize_yaw(player->eye_angles().y);
}

float resolver_t::get_forward_yaw(cs_player_t* player) {
	return math::normalize_yaw(get_backward_yaw(player) - 180.f);
}

float resolver_t::get_away_angle(cs_player_t* player) {
	return math::calculate_angle(globals->m_local->eye_position(), player->eye_position()).y;
}

float resolver_t::get_backward_yaw(cs_player_t* player) {
	return math::calculate_angle(globals->m_local->origin(), player->origin()).y;
}

float resolver_t::get_left_yaw(cs_player_t* player) {
	return math::normalize_yaw(math::calculate_angle(globals->m_local->origin(), player->origin()).y - 90.f);
}

float resolver_t::get_right_yaw(cs_player_t* player) {
	return math::normalize_yaw(math::calculate_angle(globals->m_local->origin(), player->origin()).y + 90.f);
}


int resolver_t::determine_initial_side(cs_player_t* player) {

	float angle_to_player = math::calculate_angle(globals->m_local->origin(), player->origin()).y;
	return (angle_to_player > 0) ? 1 : -1;
}


int resolver_t::determine_air_side(resolver_context_t& ctx) {

	auto velocity = ctx.m_record->m_velocity;
	if (velocity.y > 0) {
		return 1;
	} else {
		return -1;
	}
}


int resolver_t::determine_freestand_side(resolver_context_t& ctx) {

	float angle_to_player = math::calculate_angle(globals->m_local->origin(), ctx.m_player->origin()).y;
	return (angle_to_player > 0) ? 1 : -1;
}

void resolver_t::on_create_move() {
	if (!globals->m_local_alive || !settings->ragebot.enable) {
		for (int i = 0; i < 64; ++i) {
			m_info[i].reset();
		}
		return;
	}

	static int old_side[65] = {};

	for (auto& [player, entry]: entities->m_players) {
		if (!player->is_alive() || player->dormant() || !player->is_player() || player->is_teammate() || player == globals->m_local) {
			continue;
		}

		int index = player->index();

		if (old_side[index] == 0) {
			m_freestand_side[index] = determine_initial_side(player);
		} else {
			m_freestand_side[index] = old_side[index];
		}
	}

	std::memcpy(old_side, m_freestand_side, sizeof(int) * 64);
}

bool resolver_t::can_use_animations(resolver_context_t& ctx) {
	if (ctx.m_record->m_previous == nullptr) {
		return false;
	}

	auto speed = ctx.m_record->m_velocity.length2d();
	const auto last_speed = ctx.m_record->m_previous->m_velocity.length2d();
	auto& info = m_info[ctx.m_index];

	const auto original_rate = ctx.m_record->m_layers[6].m_playback_rate;
	const auto zero_rate = ctx.m_record->m_zero_layers[6].m_playback_rate;
	const auto left_delta = std::abs(ctx.m_record->m_left_layers[6].m_playback_rate - original_rate);
	const auto right_delta = std::abs(ctx.m_record->m_right_layers[6].m_playback_rate - original_rate);

	if (speed > 10.f) {
		if (!(left_delta > 0.004f && right_delta > 0.004f)) {
			if (right_delta > left_delta) {
				info.m_last_side = 1;
			} else if (right_delta < left_delta) {
				info.m_last_side = -1;
			} else if (std::abs(zero_rate - original_rate) >= 0.0015f) {
				info.m_last_side = 0;
			}
		}
	} else {
		return false;
	}

	info.m_side = info.m_last_side;
	return true;
}

bool resolver_t::jitter_fix(resolver_context_t& ctx) {
	if (ctx.m_record->m_previous == nullptr) {
		return false;
	}

	auto& jitter = ctx.m_info->m_jitter;

	if (ctx.m_record->m_eye_angles.x < 45.f || ctx.m_previous == nullptr) {
		++jitter.m_static_ticks;
		jitter.m_switch_count = 0;
		return false;
	}

	const float eye_delta = math::normalize_yaw(ctx.m_record->m_eye_angles.y - ctx.m_record->m_previous->m_eye_angles.y);
	const float abs_eye_delta = std::abs(eye_delta);

	if (abs_eye_delta <= JITTER_BEGIN_ANGLE) {
		jitter.m_switch_count = 0;
		++jitter.m_static_ticks;
	} else {
		++jitter.m_switch_count;
		jitter.m_static_ticks = 0;
	}

	if (jitter.m_static_ticks > JITTER_CACHE_SIZE || jitter.m_switch_count > MAX_SWITCH_COUNT) {
		return false;
	}

	const auto inner_delta = math::normalize_yaw(ctx.m_record->m_eye_angles.y - ctx.m_previous->m_eye_angles.y);
	ctx.m_info->m_side = inner_delta > 0.f ? -1 : 1;
	return true;
}

void resolver_t::check_inverse_side(resolver_context_t& ctx) {
	const auto sideways_left = std::abs(math::normalize_yaw(ctx.m_angle - get_left_yaw(ctx.m_player))) < 45.f;
	const auto sideways_right = std::abs(math::normalize_yaw(ctx.m_angle - get_right_yaw(ctx.m_player))) < 45.f;

	const auto forward = std::abs(math::normalize_yaw(ctx.m_angle - get_forward_yaw(ctx.m_player))) < 45.f;

	ctx.m_inverse_side = (forward || sideways_right) && !sideways_left;
	ctx.m_info->m_using_sideways = sideways_left || sideways_right;
}

bool resolver_t::air_fix(resolver_context_t& ctx) {
	if (ctx.m_info->m_ground_ticks > 1)
		return false;

	const auto velocity = ctx.m_record->m_velocity;
	const auto speed = velocity.length2d();

	if (speed > AIR_SPEED_THRESHOLD) {
		ctx.m_info->m_side = determine_air_side(ctx);
	} else {
		ctx.m_info->m_side = 0;
	}

	return true;
}

void resolver_t::apply_edge(resolver_context_t& ctx) {
	if (m_freestand_side[ctx.m_index] != 0) {
		ctx.m_info->m_side = determine_freestand_side(ctx);
	} else {
		ctx.m_info->m_side = 0;
	}
}

void resolver_t::apply(resolver_info_t* info) {
	info->m_mode.set_additional_info((info->m_side == 0 ? 1 : 0) | ((info->m_side == 1 ? 1 : 0) << 1));
}

void resolver_t::reset(int idx) {
	m_info[idx].reset();
}

bool resolver_t::run(lag_record_t* record, lag_record_t* previous) {
	auto player = record->m_player;
	int index = player->index();
	auto& info = m_info[index];
	info.m_jitter.m_should_fix = false;

	auto& mode = info.m_mode;
	mode.reset();

	auto animstate = player->animstate();
	if (!animstate || !settings->ragebot.enable || !globals->m_local->is_alive()) {
		mode.set(e_resolver_mode::off, 1);
		info.m_side = 0;
		apply(&info);
		return false;
	}

	resolver_context_t ctx{};
	ctx.m_player = player;
	ctx.m_index = index;
	ctx.m_record = record;
	ctx.m_previous = previous;
	ctx.m_info = &info;

	check_inverse_side(ctx);

	if (jitter_fix(ctx)) {
		apply(&info);
		return true;
	}

	if (air_fix(ctx)) {
		apply(&info);
		return true;
	}

	if (can_use_animations(ctx)) {
		apply(&info);
		return true;
	}

	if (ctx.m_info->m_side == 0) {
		apply_edge(ctx);
	}

	apply(&info);
	return true;
}

void resolver_t::force_off(cs_player_t* player) {
	int idx = player->index();
	auto& info = m_info[idx];

	info.m_mode.reset();
	info.m_mode.set(e_resolver_mode::off, 4);

	apply(&info);
}

void resolver_t::apply_resolver_info(lag_record_t* anims) {
	std::memcpy(&anims->m_resolver, &m_info[anims->m_player->index()], sizeof(resolver_info_t));
}