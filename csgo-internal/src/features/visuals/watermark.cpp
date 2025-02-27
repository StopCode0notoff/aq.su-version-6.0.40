#include "watermark.hpp"
#include "../../../deps/imgui/imgui.h"
#include "../../blur/blur.hpp"
#include "../../game/override_entity_list.hpp"
#include "../../globals.hpp"

static gui::instance::window_t* window = nullptr;
//static gui::instance::window_t* window2 = nullptr;

auto lerp_float_ = [](float& value, float target) {
	if (value > target)
		value = std::clamp(value - std::abs(value - target) * gui::globals().time_delta * 9.f, target, FLT_MAX);
	else if (value < target)
		value = std::clamp(value + std::abs(value - target) * gui::globals().time_delta * 9.f, -FLT_MAX, target);
};

static void render_watermark(gui::render_context_t& ctx, gui::instance::window_t* window) {
	using namespace gui;

	constexpr auto header_size = 20.f;
	const auto font = fonts::menu_main; 

	vec2d window_size = { 220.f, 45.f }; 
	static vec2d visual_size = window->get_size();

	shaders::create_blur(ImGui::GetBackgroundDrawList(),
						 to_imvec2(ctx.m_base_position),
						 to_imvec2(ctx.m_base_position + visual_size),
						 color_t{ 127, 127, 127 }.modify_alpha(ctx.m_alpha).abgr(), dpi::scale(5.f));

	const vec2d pos = ctx.m_base_position;

	render::filled_rect(pos, { visual_size.x, dpi::scale(header_size) },
						styles::get().window_backround.modify_alpha(ctx.m_alpha), dpi::scale(5.f), ImDrawCornerFlags_Top);

	auto clr1 = color_t(200, 200, 255).modify_alpha(ctx.m_alpha);
	auto clr2 = color_t(100, 100, 255).modify_alpha(ctx.m_alpha);
	render::filled_rect_gradient(pos.x, pos.y + dpi::scale(header_size), visual_size.x, dpi::scale(2.f), clr1, clr2, clr2, clr1);

	//static inline std::string username = STRC("");
	 //const char frametime = interfaces::global_vars->m_framecount;
	std::string wt_text = STRC("[ @AstralQuo ] | Version 6  "); //+ /* username  + */ STRC("] | [TIME: ") /* + frametime */ + STRC("]");
	render::text(pos.x + dpi::scale(10.f), pos.y + dpi::scale(header_size) * 0.5f,
				 color_t(255, 255, 255).modify_alpha(ctx.m_alpha), render::none, font, wt_text);

	lerp_float_(visual_size.x, window_size.x);
	lerp_float_(visual_size.y, window_size.y);
	window->set_size(visual_size);
}


/* void watermark_t::render() {
	if (window == nullptr) {
		window = new gui::instance::window_t{ { gui::dpi::scale(109.f), 0.f }, { 200.f, 500.f } };
		window->override_render_function(render_watermark);
		window->m_opened = true;
		gui::add_instance(window);
	} */
	//if (window2 == nullptr) {
	////	window2 = new gui::instance::window_t{ { gui::dpi::scale(80.f), 0.f }, { 200.f, 500.f } };
	//	window2->override_render_function(render_watermark);
	//	window2->m_opened = true;
	//	gui::add_instance(window2);
	//}
//}
