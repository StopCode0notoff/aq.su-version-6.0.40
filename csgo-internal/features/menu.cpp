#include "menu.hpp"
#include <mutex>
#include "misc.hpp"
#include "../interfaces.hpp"
#include "../../deps/weave-gui/include.hpp"
#include "../globals.hpp"
#include "hvh/hvh.hpp"
#include "../../bytes/weave_logo.h"
#include "../../bytes/button_color.h"
#include "../blur/blur.hpp"
#include "../features/visuals/skin_changer.hpp"
//#include "..\sec_include\secure.hpp"

#include "../../bytes/rage_icon.h"

using namespace gui;

namespace menu {

	ImTextureID logo, button_image;

	static void welcome_tab() {
		ImGui::Text(STRC("Welcome back!\nThis is free version AstralQuo.su\n\nUpdated: %s %s"), STRC(__DATE__), STRC(__TIME__));
		ImGui::Text(STRC("AstralQuo v6.0.4.1 Stable"));
		ImGui::Text(STRC("\n\nCreators:\n StopCode\n No_data (aka StopCode)\n DragonBase (aka StopCode)\n siotasuke (aka StopCode)\n"));
		ImGui::Text(STRC("Testers:\n Naalbik (aka ANALbik (aka Serge Noalbandian))\n William1337 (?)\n Samrich\n laktoza1337\n\n"));
		ImGui::Text(STRC("CHANGELOG 2.5"));
		ImGui::SameLine();
		ImGui::TextDisabled("[?]");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("[~] Fix menu\n[~] Improved resolver\n[-] Knifebot bad working");
		}
		ImGui::Text(STRC("CHANGELOG 3.0"));
		ImGui::SameLine();
		ImGui::TextDisabled("[?]");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("[~] Improve prediction\n[~] Improve resovler\n[~] Improve hitchance\n[+] Menu blur");
		}
		//ImGui::Text(STRC("CHANGELOG 3.1"));
		//ImGui::SameLine();
		//ImGui::TextDisabled("[?]");
		//if (ImGui::IsItemHovered()) {
		//	ImGui::SetTooltip("[~] EXPEREMENTAL RESOLVER");
		//}
		ImGui::Text(STRC("CHANGELOG 3.2"));
		ImGui::SameLine();
		ImGui::TextDisabled("[?]");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("[+] Computing limit tooltip");
		}
		//ImGui::Text(STRC("CHANGELOG 3.3"));
		//ImGui::SameLine();
		//ImGui::TextDisabled("[?]");
		//if (ImGui::IsItemHovered()) {
		//	ImGui::SetTooltip("[~] Improve resolver");
		//}
		ImGui::Text(STRC("CHANGELOG 3.4"));
		ImGui::SameLine();
		ImGui::TextDisabled("[?]");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("[+] Model changer\n..counter-strike/csgo/materials/models/player/custom_player/");
		}
		ImGui::Text(STRC("CHANGELOG 3.5"));
		ImGui::SameLine();
		ImGui::TextDisabled("[?]");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("[~] Rework resovler");
		}
		ImGui::Text(STRC("CHANGELOG 3.6"));
		ImGui::SameLine();
		ImGui::TextDisabled("[?]");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("[~] Rework ragebot");
		}
		ImGui::Text(STRC("CHANGELOG 4.0"));
		ImGui::SameLine();
		ImGui::TextDisabled("[?]");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("[~] Improve Prediction\n[~] Rework resolver");
		}
	}

	static void ragebot_tab() {
		static int group_tab = 0;

		//	ImGui::Image(rage_icon, ImVec2(32, 32)); 
		//ImGui::SameLine();
		//	ImGui::Text("Ragebot");

		ImGui::Columns(2, nullptr, false);

		/*ImGui::SetColumnWidth(0, 240.f);

		ImGui::SetColumnWidth(1, 240.f);*/

		begin_child(STR("| General"), 215.f);
		{
			checkbox(STRC("Enable"), &settings->ragebot.enable);
			if (settings->ragebot.enable) {
				checkbox(STRC("Auto-fire"), &settings->ragebot.autofire);
				checkbox(STRC("Silent"), &settings->ragebot.silent);
				combo(STR("Computing limit"), &settings->ragebot.computing_limit, { STR("Off"), STR("Medium"), STR("High") });

				ImGui::SameLine();
				ImGui::TextDisabled("[?]");

				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Computing limit is a function that limits the resources of your PC for processing a ragebot. When the \"off\" option is selected, the PC resources will be used to the maximum.");
				}

				checkbox(STRC("Multi-threading"), &settings->multithreading);
				//ImGui::Spacing;
				ImGui::Separator();
				//ImGui::Spacing;
			}
			//const char* resolver_modes[] = { "Off", "Edge", "Anim", "Auto" };
			//ImGui::Combo("Resolver Mode", reinterpret_cast<int*>(&settings->ragebot.resolver_mode), resolver_modes, IM_ARRAYSIZE(resolver_modes));
		}
		end_child();

		ImGui::NextColumn();

		begin_child(STR("| Weapons"), 725.f);
		{
			if (settings->ragebot.enable) {
				combo(STR("Weapon group"), &group_tab, { STR("Default"), STR("Snipers"), STR("Auto-snipers"), STR("Heavy pistols"), STR("Pistols"), STR("Rifles"), STR("Heavies"), STR("Shotguns"), STR("SMGs") });

				group_tab = std::clamp(group_tab, 0, 8);
				auto weapon = &settings->ragebot.weapons[group_tab];

				if (group_tab > 0) {
					switch (group_tab) {
						case incheat_vars::rage_group_snipers:
							combo(STR("Current weapon"), &weapon->tab, { STR("Default"), STR("SSG-08"), STR("AWP") });
							break;

						case incheat_vars::rage_group_autosnipers:
							combo(STR("Current weapon"), &weapon->tab, { STR("Default"), STR("G3SG1"), STR("SCAR-20") });
							break;

						case incheat_vars::rage_group_heavy_pistols:
							combo(STR("Current weapon"), &weapon->tab, { STR("Default"), STR("Desert Eagle"), STR("Revolver") });
							break;

						case incheat_vars::rage_group_pistols:
							combo(STR("Current weapon"), &weapon->tab,
								  { STR("Default"), STR("Glock-18"), STR("P2000"), STR("USP-S"), STR("Dual Berettas"), STR("P250"), STR("Tec-9"), STR("Five-Seven"), STR("CZ-75") });
							break;

						case incheat_vars::rage_group_rifles:
							combo(STR("Current weapon"), &weapon->tab,
								  { STR("Default"), STR("Galil AR"), STR("Famas"), STR("AK-47"), STR("M4A4"), STR("M4A1-S"), STR("SG-553"), STR("AUG") });
							break;

						case incheat_vars::rage_group_heavies:
							combo(STR("Current weapon"), &weapon->tab, { STR("Default"), STR("M249"), STR("Negev") });
							break;

						case incheat_vars::rage_group_shotguns:
							combo(STR("Current weapon"), &weapon->tab, { STR("Default"), STR("Nova"), STR("XM1014"), STR("Sawed-off"), STR("MAG-7") });
							break;

						case incheat_vars::rage_group_smgs:
							combo(STR("Current weapon"), &weapon->tab,
								  { STR("Default"), STR("Mac-10"), STR("MP9"), STR("MP7"), STR("MP5-SD"), STR("UMP-45"), STR("P90"), STR("PP-Bizon") });
							break;
					}

					if (weapon->tab == 0)
						checkbox(STRC("Override group"), &weapon->override_default);
				} else
					weapon->tab = 0;

				auto set = &weapon->settings[weapon->tab];

				if (group_tab > 0 && weapon->tab != 0)
					checkbox(STRC("Override weapon"), &set->override_default);

				if (group_tab == 0 || settings->ragebot.weapons[group_tab].override_default || set->override_default) {

					//checkbox(STRC("Quick stop in air"), &set->quick_stop_in_air);

					checkbox(STRC("Quick stop"), &set->quick_stop);
					if (set->quick_stop) {
						multicombo(STR("Options"), &set->quick_stop_options, { STR("Predictive"), STR("Force accuracy"), STR("Between shots") });
					}

					if (group_tab == 1 || group_tab == 2) {
						checkbox(STRC("Automatic scope"), &set->autoscope);
					}
					//checkbox(STRC("Visible only"), &set->visible_only);

					multicombo(STR("Hitboxes"), &set->hitboxes, {
																		STR("Head"),
																		STR("Upper chest"),
																		STR("Chest"),
																		STR("Lower chest"),
																		STR("Pelvis"),
																		STR("Stomach"),
																		STR("Legs"),
																		STR("Feet"),
																		STR("Arms"),
																});

					slider_int(STR("Hit chance"), &set->hitchance, 0, 100, STR("%d%%"));

					if (set->hitchance > 0)
						checkbox(STRC("Strict hitchance"), &set->strict_hitchance);

					slider_int(STR("Minimum damage"), &set->mindamage, 0, 120, /* set->mindamage > 100 ? STR("Health + %dhp") :  */ STR("%dhp"));

					if (hotkeys->override_damage.is_valid())
						slider_int(STR("Minimum damage override"), &set->mindamage_override, 0, 120, STR("%dhp"));

					slider_int(STR("Head scale"), &set->head_scale, 0, 100, set->head_scale == 0 ? STR("Auto") : STR("%d%%"));
					slider_int(STR("Body scale"), &set->body_scale, 0, 100, set->body_scale == 0 ? STR("Auto") : STR("%d%%"));

					combo(STR("Priority hitgroup"), &set->priority_hitgroup, { STR("Off"), STR("Head"), STR("Pelvis/Stomach"), STR("Chest") });

					multicombo(STR("Force safepoint"), &set->force_safepoint, {
																					  STR("Head"),
																					  STR("Body"),
																					  STR("Limbs"),
																			  });

					checkbox(STRC("Prefer safepoint"), &set->prefer_safepoint);

					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();

					if (group_tab > 0) {
						if (ImGui::Button(STRC("Import from default"))) {
							std::memcpy(set, &settings->ragebot.weapons[0].settings[0], sizeof(incheat_vars::ragebot_settings_t));
							set->override_default = true;
						}

						if (weapon->settings[0].override_default) {
							if (ImGui::Button(STRC("Import from group"))) {
								std::memcpy(set, &weapon->settings[0], sizeof(incheat_vars::ragebot_settings_t));
								set->override_default = true;
							}
						}
					}
				}
			}
		}
		end_child();

		//ImGui::NextColumn();

		/* begin_child(STR("| Anti-aim"), 814.f);
		{
			static int trigger_tab = 0;
			checkbox(STRC("Enable"), &settings->antiaim.enable);
			checkbox(STRC("Defensive"), &settings->exploits.defensive_enable);
			if (settings->exploits.defensive_enable) {
				multicombo(STR("Defensive options"), &settings->exploits.defensive_flags, { STR("On ground"), STR("In air"), STR("Safe peek"), STR("Anti Aim") });
			}

			checkbox(STRC("Immediate teleport"), &settings->exploits.immediate_teleport);

			if (settings->antiaim.enable) {
				combo(STR("Trigger"), &trigger_tab, { STR("Default"), STR("Standing"), STR("Moving"), STR("In air"), STR("Slow walking"), STR("Crouching"), STR("Defensive"), STR("On use") });
				auto set = &settings->antiaim.triggers[trigger_tab];
				if (trigger_tab > 0)
					checkbox(STRC("Override default"), &set->override_default);

				if (trigger_tab == 0 || set->override_default) {
					combo(STR("Override pitch"), &set->pitch, { STR("Off"), STR("Down"), STR("Random"), STR("Jitter"), STR("Zero"), STR("Up"), STR("Custom") });
					if (set->pitch == 6) {
						slider_int(STR("Value"), &set->custom_pitch, -89, 89);
					}

					checkbox(STRC("Force off"), &set->force_off);
					checkbox(STRC("Ignore freestand"), &set->ignore_freestand);
					checkbox(STRC("Ignore manuals"), &set->ignore_manuals);

					slider_int(STR("Desync"), &set->desync_amount, 0, 100, STR("%d%%"));
					checkbox(STRC("Randomize desync"), &set->randomize_desync);
					combo(STR("Align by target"), &set->align_by_target, { STR("Off"), STR("When on screen"), STR("Always") });

					slider_int(STR("Yaw add"), &set->yaw_add, -180, 180);

					//combo(STR("Advanced type"), &settings->jitter_type, { STR("None"), STR("Offset"), STR("3-Way"), STR("Center"), STR("Random") });
					combo(STR("Jitter type"), &set->jitter_type, { STR("None"), STR("Offset"), STR("Center"), ("Random") });

					if (set->jitter_type > 0)
						slider_int(STR("Jitter range"), &set->jitter_angle, -180, 180);
					if (set->jitter_angle > 2 || set->jitter_angle < -2)            //
						checkbox(STRC("Randomize jitter"), &set->randomize_jitter); //

					checkbox(STRC("Align desync"), &set->align_desync);
					checkbox(STRC("Desync jitter"), &set->desync_jitter);

					checkbox(STRC("Spin"), &set->spin);

					if (set->spin) {
						slider_int(STR("Spin speed"), &set->spin_speed, 1, 360);
						slider_int(STR("Spin range"), &set->spin_range, 2, 360);
						//ImGui::Separator(); //
					}
					ImGui::Separator(); //
										//checkbox(STRC("Edge yaw on peek"), &set->edge_yaw_on_peek);
				}
			}
		}
		end_child();

		begin_child(STR("| Packet latency"), 200.f);
		{
			combo(STR("Fake-lag"), &settings->antiaim.fakelag, { STR("Off"), STR("Static"), STR("Dynamic") });
			slider_int(STR("Fake-lag ticks"), &settings->antiaim.fakelag_limit, 0, MAX_FAKELAG, STR("       %.0f packets"));
		}
		end_child(); */
	}

	static void antiaim_tab() {
		static int group_tab = 0;

		ImGui::Columns(2, nullptr, false);

		begin_child(STR("| Anti-aim"), 814.f);
		{
			static int trigger_tab = 0;
			checkbox(STRC("Enable"), &settings->antiaim.enable);
			checkbox(STRC("Defensive"), &settings->exploits.defensive_enable);
			if (settings->exploits.defensive_enable) {
				multicombo(STR("Defensive options"), &settings->exploits.defensive_flags, { STR("On ground"), STR("In air"), STR("Safe peek"), STR("Anti Aim") });
			}

			checkbox(STRC("Immediate teleport"), &settings->exploits.immediate_teleport);

			if (settings->antiaim.enable) {
				combo(STR("Trigger"), &trigger_tab, { STR("Default"), STR("Standing"), STR("Moving"), STR("In air"), STR("Slow walking"), STR("Crouching"), STR("Defensive"), STR("On use") });
				auto set = &settings->antiaim.triggers[trigger_tab];
				if (trigger_tab > 0)
					checkbox(STRC("Override default"), &set->override_default);

				if (trigger_tab == 0 || set->override_default) {
					combo(STR("Override pitch"), &set->pitch, { STR("Off"), STR("Down"), STR("Random"), STR("Jitter"), STR("Zero"), STR("Up"), STR("Custom") });
					if (set->pitch == 6) {
						slider_int(STR("Value"), &set->custom_pitch, -89, 89);
					}

					checkbox(STRC("Force off"), &set->force_off);
					checkbox(STRC("Ignore freestand"), &set->ignore_freestand);
					checkbox(STRC("Ignore manuals"), &set->ignore_manuals);

					slider_int(STR("Desync"), &set->desync_amount, 0, 100, STR("%d%%"));
					checkbox(STRC("Randomize desync"), &set->randomize_desync);
					combo(STR("Align by target"), &set->align_by_target, { STR("Off"), STR("When on screen"), STR("Always") });

					slider_int(STR("Yaw add"), &set->yaw_add, -180, 180);

					//combo(STR("Advanced type"), &settings->jitter_type, { STR("None"), STR("Offset"), STR("3-Way"), STR("Center"), STR("Random") });
					combo(STR("Jitter type"), &set->jitter_type, { STR("None"), STR("Offset"), STR("Center"), ("Random") });

					if (set->jitter_type > 0)
						slider_int(STR("Jitter range"), &set->jitter_angle, -180, 180);
					if (set->jitter_angle > 2 || set->jitter_angle < -2)			//
						checkbox(STRC("Randomize jitter"), &set->randomize_jitter); //

					checkbox(STRC("Align desync"), &set->align_desync);
					checkbox(STRC("Desync jitter"), &set->desync_jitter);

					checkbox(STRC("Spin"), &set->spin);

					if (set->spin) {
						slider_int(STR("Spin speed"), &set->spin_speed, 1, 360);
						slider_int(STR("Spin range"), &set->spin_range, 2, 360);
						//ImGui::Separator(); //
					}
					ImGui::Separator(); //
										//checkbox(STRC("Edge yaw on peek"), &set->edge_yaw_on_peek);
				}
			}
		}
		end_child();

		ImGui::NextColumn();

		begin_child(STR("| Packet latency"), 200.f);
		{
			combo(STR("Fake-lag"), &settings->antiaim.fakelag, { STR("Off"), STR("Static"), STR("Dynamic") });
			slider_int(STR("Fake-lag ticks"), &settings->antiaim.fakelag_limit, 0, MAX_FAKELAG, STR("       %.0f packets"));
		}
		end_child();
	}


	namespace esp {
		static void glow_settings(std::string name, incheat_vars::glow_esp_settings_t& set) {
			checkbox_with_color(name + STR("##") + std::to_string((uintptr_t)&set), &set.enable, set.color.base());
		}
	} // namespace esp

	static void players_tab() {
		ImGui::Columns(2, nullptr, false);

		begin_child(STR("| ESP"));
		{
			checkbox(STRC("Enable"), &settings->player_esp.enable);
			if (settings->player_esp.enable) {
				checkbox_with_color(STR("Box"), &settings->player_esp.box, settings->player_esp.box_color.base());
				text_settings(STR("Name"), settings->player_esp.name, button_image);
				bar_settings(STR("Health"), settings->player_esp.health, button_image);
				bar_settings(STR("Ammo"), settings->player_esp.ammo, button_image);

				text_array_settings(STR("Flags"),
									{ STR("Helmet & Kevlar"), STR("Scope"), STR("Flashed"), STR("Defuser"), STR("Distance"), STR("Ping"), STR("Break LC"), STR("Hittable"), STR("Bomb") },
									settings->player_esp.flags, button_image);

				text_array_settings(STR("Weapon"), { STR("Name"), STR("Icon") }, settings->player_esp.weapon, button_image);
			}
		}
		end_child();

		ImGui::NextColumn();

		begin_child(STR("| Models"));
		{
			static int chams_tab = 0;

			combo(STR("Chams"), &chams_tab, {
													STRS("Enemy Visible"),
													STRS("Enemy NotVisible"),
													STRS("Local player"),
													STRS("Desync model"),
													STRS("Enemy Backtrack"),
													STRS("Shot"),
													STRS("Viewmodel arms"),
													STRS("Viewmodel weapon"),
													STRS("Local attachments"),
											});

			auto& set = settings->player_esp.chams[chams_tab];
			checkbox(STRC("Enable"), &set.enable);

			if (chams_tab == 2) {
				checkbox(STRC("Transparent in scope"), &settings->player_esp.local_blend_in_scope);

				if (settings->player_esp.local_blend_in_scope)
					slider_int(STR("Transparency amount"), &settings->player_esp.local_blend_in_scope_amount, 0, 100, STR("%d%%"));
			}

			if (set.enable) {
				if ((chams_tab == 3 || chams_tab == 4) && !settings->ragebot.enable) {
					ImGui::Text(STRC("Please turn on ragebot before setup this feature!"));
				} else {
					if (chams_tab == 4)
						checkbox(STRC("Last shot only"), &settings->player_esp.shot_chams_last_only);

					combo(STR("Material"), &set.material, { STR("Default"), STR("Flat"), STR("Metallic"), STR("Randomized") });

					ImGui::ColorEdit4(STRC("Material color"), set.color.base(), ImGuiColorEditFlags_AlphaBar);

					if (set.material == 2) {
						ImGui::ColorEdit4(STRC("Metallic color"), set.metallic_color.base(), ImGuiColorEditFlags_AlphaBar);

						slider_int(STR("Phong"), &set.phong_amount, 0, 100, STR("%d%%"));
						slider_int(STR("Rim"), &set.rim_amount, -100, 100, STR("%d%%"));
					}

					const std::vector<std::string> overlays_name = {
						STR("Glow fade"),
						STR("Glow line"),
						STR("Wireframe"),
						STR("Glass"),
						STR("Animated")
					};

					multicombo(STR("Overlays"), &set.overlays, overlays_name);

					for (uint8_t i = 0; i < overlays_name.size(); ++i)
						if (set.overlays.at(i))
							ImGui::ColorEdit4((overlays_name[i] + STR(" color")).c_str(), set.overlays_colors[i].base(), ImGuiColorEditFlags_AlphaBar);
				}
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox(STRC("Enemy glow"), &settings->player_esp.glow.enable);
			ImGui::SameLine();
			ImGui::Text(STRC("Enemy glow"));
			if (settings->player_esp.glow.enable) {
				ImGui::SameLine();
				ImGui::ColorEdit4((STR("##Enemy")).c_str(), settings->player_esp.glow.color.base(), ImGuiColorEditFlags_AlphaBar);
			}
			ImGui::Spacing();

			ImGui::Checkbox(STRC("Local glow"), &settings->player_esp.local_glow.enable);
			ImGui::SameLine();
			ImGui::Text(STRC("Local glow"));
			if (settings->player_esp.local_glow.enable) {
				ImGui::SameLine();
				ImGui::ColorEdit4((STR("##Local")).c_str(), settings->player_esp.local_glow.color.base(), ImGuiColorEditFlags_AlphaBar);
			}
			ImGui::Spacing();
		}
		end_child();
	}

	static void world_tab() {
		ImGui::Columns(2, nullptr, false);

		begin_child(STR("| World modulation"), 270.f);
		{
			checkbox(STRC("Night mode"), &settings->visuals.nightmode.enable);
			if (settings->visuals.nightmode.enable) {
				ImGui::ColorEdit3(STRC("World color"), settings->visuals.nightmode.color.base());
				ImGui::ColorEdit3(STRC("Skybox color"), settings->visuals.nightmode.skybox_color.base());
				ImGui::ColorEdit4(STRC("Props color"), settings->visuals.nightmode.prop_color.base(), ImGuiColorEditFlags_AlphaBar);
			}
			checkbox(STRC("Sunset mode"), &settings->visuals.sunsetmode.enable);
			if (settings->visuals.sunsetmode.enable) {
				slider_int(STR("Sun pitch"), &settings->visuals.sunsetmode.sunset_angle_x, -180, 180);
				slider_int(STR("Sun yaw"), &settings->visuals.sunsetmode.sunset_angle_y, -180, 180);
			}
			checkbox(STRC("Fullbright"), &settings->visuals.fullbright);
			ImGui::Separator();
		}
		end_child();

		begin_child(STR("| Weapons & Projectiles"), 275.f);
		{
			checkbox(STRC("Weapon ESP"), &settings->weapon_esp.enable);
			if (settings->weapon_esp.enable) {
				checkbox_with_color(STR("Box"), &settings->weapon_esp.box, settings->weapon_esp.box_color.base());

				text_array_settings(STR("Name"), { STR("Text"), STR("Icon") }, settings->weapon_esp.name, button_image);
				bar_settings(STR("Ammo"), settings->weapon_esp.ammo, button_image);

				ImGui::Checkbox(STRC("Glow"), &settings->weapon_esp.glow.enable);
				ImGui::SameLine();
				ImGui::Text(STRC("Glow"));
				if (settings->weapon_esp.glow.enable) {
					ImGui::SameLine();
					ImGui::ColorEdit4((STR("##Glow_weapon")).c_str(), settings->weapon_esp.glow.color.base(), ImGuiColorEditFlags_AlphaBar);
				}
				ImGui::Spacing();
				//esp::glow_settings(STR("Glow"), settings->weapon_esp.glow); //
				//ImGui::Spacing();
			}

			ImGui::Separator();

			ImGui::Spacing();

			checkbox(STRC("Projectiles warning"), &settings->grenade_esp.enable);
			//checkbox(STRC("Projectiles prediction"), &settings->grenade_esp.prediction);
			ImGui::Checkbox(STRC("Projectiles prediction"), &settings->grenade_esp.prediction);
			ImGui::SameLine();
			ImGui::Text(STRC("Projectiles prediction"));
			if (settings->grenade_esp.prediction) {
				ImGui::SameLine();
				ImGui::ColorEdit4((STR("##ProjectilePredict")).c_str(), settings->grenade_esp.prediction_color.base(), ImGuiColorEditFlags_AlphaBar);
			}
			ImGui::Spacing();
			//checkbox(STRC("Bomb ESP"), &settings->bomb_esp.enable);
		}
		end_child();

		ImGui::NextColumn();

		begin_child(STR("| Other"), 860.f);
		{
			static std::vector<std::string> knife_models = {
				STR("Default"), STR("Bayonet"), STR("Bowie Knife"), STR("Butterfly Knife"),
				STR("Falchion Knife"), STR("Flip Knife"), STR("Gut Knife"),
				STR("Huntsman Knife"), STR("Karambit"), STR("M9 Bayonet"),
				STR("Shadow Daggers"), STR("Navaja Knife"), STR("Stiletto Knife"),
				STR("Ursus Knife"), STR("Talon Knife"), STR("Classic Knife"),
				STR("Skeleton Knife"), STR("Nomad Knife"), STR("Survival Knife"), STR("Paracord Knife")
			};

			static const std::vector<std::string> agent_skins = {
				STR("Default"),
				STR("Special Agent Ava"),
				STR("Operator"),
				STR("Markus Delrow"),
				STR("Michael Syfers"),
				STR("B Squadron Officer"),
				STR("Seal Team 6 Soldier"),
				STR("Buckshot"),
				STR("Lt. Commander Ricksaw"),
				STR("Third Commando Company"),
				STR("'Two Times' McCoy"),
				STR("Dragomir"),
				STR("Rezan The Ready"),
				STR("'The Doctor' Romanov"),
				STR("Maximus"),
				STR("Blackwolf"),
				STR("The Elite Mr. Muhlik"),
				STR("Ground Rebel"),
				STR("Osiris"),
				STR("Prof. Shahmat"),
				STR("Enforcer"),
				STR("Slingshot"),
				STR("Soldier"),
				STR("Street Soldier"),
				STR("'Blueberries' Buckshot"),
				STR("'Two Times' McCoy"),
				STR("Rezan the Redshirt"),
				STR("Dragomir"),
				STR("Cmdr. Mae 'Dead Cold' Jamison"),
				STR("001st Lieutenant Farlow"),
				STR("John 'Van Healen' Kask"),
				STR("Bio-Haz Specialist"),
				STR("Sergeant Bombson"),
				STR("Chem-Haz Specialist"),
				STR("Sir Bloody Miami Darryl"),
				STR("Sir Bloody Silent Darryl"),
				STR("Sir Bloody Skullhead Darryl"),
				STR("Sir Bloody Darryl Royale"),
				STR("Sir Bloody Loudmouth Darryl"),
				STR("Safecracker Voltzmanns"),
				STR("Little Kev"),
				STR("Number K"),
				STR("Getaway Sally"),
				STR("Cmdr. Davida 'Goggles' Fernandez"),
				STR("Cmdr. Frank 'Wet Sox' Baroud"),
				STR("Lieutenant Rex Krikey"),
				STR("Sous-Lieutenant Medic"),
				STR("Chem-Haz Capitaine"),
				STR("Chef d'Escadron Rouchard"),
				STR("Aspirant"),
				STR("Officer Jacques Beltram"),
				STR("D Squadron Officer"),
				STR("Primeiro Tenente"),
				STR("Lieutenant 'Tree Hugger' Farlow"),
				STR("Bloody Darryl The Strapped"),
				STR("Mr. Muhlik"),
				STR("Trapper"),
				STR("Trapper Aggressor"),
				STR("Vypa Sista of the Revolution"),
				STR("Col. Mangos Dabisi"),
				STR("'Medium Rare' Crasswater"),
				STR("Crasswater The Forgotten"),
				STR("Elite Trapper Solman"),
				STR("tm_pirate"),
				STR("tm_pirate_varianta"),
				STR("tm_pirate_variantb"),
				STR("tm_pirate_variantc"),
				STR("tm_pirate_variantd"),
				STR("tm_anarchist"),
				STR("tm_anarchist_varianta"),
				STR("tm_anarchist_variantb"),
				STR("tm_anarchist_variantc"),
				STR("tm_anarchist_variantd"),
				STR("tm_balkan_varianta"),
				STR("tm_balkan_variantb"),
				STR("tm_balkan_variantc"),
				STR("tm_balkan_variantd"),
				STR("tm_balkan_variante"),
				STR("tm_jumpsuit_varianta"),
				STR("tm_jumpsuit_variantb"),
				STR("tm_jumpsuit_variantc"),
			};



			combo(STR("Override knife"), &settings->skins.knife_model, knife_models);
			combo(STR("Agent T"), &settings->skins.agent_t, agent_skins);
			combo(STR("Agent CT"), &settings->skins.agent_ct, agent_skins);

				if (skin_changer) {
 
        static bool enable_custom_model = false;

		ImGui::Text(STRC("Custom model"));
        if (ImGui::Checkbox("Enable Custom Model", &enable_custom_model))
		{

            if (!enable_custom_model) {
                skin_changer->custom_model_path.clear();
                skin_changer->reset();
            }
			ImGui::SameLine();
			ImGui::TextDisabled("[?]");

			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Drop your model to ../game_folder/csgo/models/player/custom_player/\nAnd write your_model/your_model.mdl");
			}
        }

        static char model_path[256] = ""; 
        if (enable_custom_model) {
            if (strlen(model_path) == 0) {
                strncpy_s(model_path, sizeof(model_path), skin_changer->custom_model_path.c_str(), _TRUNCATE);
            }

			ImGui::Text(STRC("Custom model path"));
            if (ImGui::InputText("Custom Model Path", model_path, sizeof(model_path))) {
				ImGui::SameLine();
				ImGui::TextDisabled("[?]");

				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Drop your model to ../game_folder/csgo/models/player/custom_player/\nAnd write your_model/your_model.mdl");
				}
                
            }

            if (ImGui::Button("Apply Model")) {
                skin_changer->custom_model_path = std::string(model_path); 
                skin_changer->reset();
            }
        }

        if (ImGui::Button("Update model")) {
            enable_custom_model = false;
            skin_changer->custom_model_path.clear(); 
            skin_changer->reset(); 
        }
    }

			combo(STR("Bullet tracer"), &settings->bullets.tracer, { STR("Off"), STR("Line"), STR("Beam"), STR("Glow"), ("Bubble"), ("TWall") });
			if (settings->bullets.tracer > 0) {
				ImGui::ColorEdit4(STRC("Tracer color"), settings->bullets.tracer_color.base());
			}

			ImGui::Checkbox(STRC("Server impacts"), &settings->bullets.server_impacts);
			ImGui::SameLine();
			ImGui::Text(STRC("Server impacts"));
			if (settings->bullets.server_impacts) {
				for (auto& color: settings->bullets.server_impact_colors) {
					ImGui::SameLine();
					ImGui::ColorEdit4((STR("##color") + std::to_string((uintptr_t)color.base())).c_str(), color.base(), ImGuiColorEditFlags_AlphaBar);
				}
			}
			ImGui::Spacing();

			ImGui::Checkbox(STRC("Client impacts"), &settings->bullets.client_impacts);
			ImGui::SameLine();
			ImGui::Text(STRC("Client impacts"));
			if (settings->bullets.client_impacts) {
				for (auto& color: settings->bullets.client_impact_colors) {
					ImGui::SameLine();
					ImGui::ColorEdit4((STR("##color") + std::to_string((uintptr_t)color.base())).c_str(), color.base(), ImGuiColorEditFlags_AlphaBar);
				}
			}
			ImGui::Spacing();

			if (settings->bullets.server_impacts || settings->bullets.client_impacts)
				slider_int(STR("Impacts size"), &settings->bullets.impacts_size, 1, 30);

			checkbox(STRC("Force crosshair"), &settings->visuals.force_crosshair);
			//checkbox(STRC("Viewmodel in scope"), &settings->visuals.viewmodel_in_scope);       //
			//checkbox(STRC("Penetration crosshair"), &settings->visuals.penetration_crosshair); //

			ImGui::Separator();
			multicombo(STR("Hitmarker"), &settings->visuals.hitmarker, { STR("World"), STR("Center of screen"), STR("Show damage") });
			multicombo(STR("Removals"), &settings->visuals.removals, { STR("Visual recoil"), STR("Smoke"), STR("Flash"), STR("Scope"), STR("Post processing"), STR("Fog"), STR("World shadow"), STR("Foot shadow"), STR("Viewmodel bob") });

			if (settings->visuals.removals.at(3)) {
				combo(STR("Override scope"), &settings->visuals.override_scope, {
																						STR("Off"),
																						STR("Default"),
																						STR("Static"),
																				});

				if (settings->visuals.override_scope > 1) {
					ImGui::ColorEdit4(STRC("Scope color-in"), settings->visuals.scope_color[0].base(), ImGuiColorEditFlags_AlphaBar);
					ImGui::ColorEdit4(STRC("Scope color-out"), settings->visuals.scope_color[1].base(), ImGuiColorEditFlags_AlphaBar);

					slider_int(STR("Scope size"), &settings->visuals.scope_size, 2, 300);
					slider_int(STR("Scope thickness"), &settings->visuals.scope_thickness, 1, 5);
					slider_int(STR("Scope gap"), &settings->visuals.scope_gap, 0, 100);
				}
			}

			slider_int(STR("Thirdperson distance"), &settings->visuals.thirdperson_dist, 0, 150);
			slider_int(STR("World FOV"), &settings->visuals.world_fov, 0, 50);
			slider_int(STR("Zoom FOV"), &settings->visuals.zoom_fov, 0, 100, STR("%d%%"));
			slider_int(STR("Viewmodel FOV"), &settings->visuals.viewmodel_fov, 0, 30, STR("%d%"));
		}
		end_child();
	}

	static void hotkeys_tab() {
		ImGui::Columns(2, nullptr, false);

		checkbox(STRC("Hotkeys list"), &settings->misc.hotkeys_list);

		if (settings->ragebot.enable) {
			ImGui::Text(STRC("Ragebot"));
			//	ImGui::Separator();
			{
				hotkey(STR("Double-tap"), &hotkeys->doubletap);
				hotkey(STR("Hide-shot"), &hotkeys->hide_shot);
				hotkey(STR("Override damage"), &hotkeys->override_damage);
				hotkey(STR("Force body"), &hotkeys->force_body);
			}
		}

		ImGui::Text(STRC("| Movement"));
		//ImGui::Separator();
		{
			hotkey(STR("Peek assist"), &hotkeys->peek_assist);
			hotkey(STR("Fake duck"), &hotkeys->fake_duck);
		}

		ImGui::Text(STRC("| Other"));
		//ImGui::Separator();
		{
			hotkey(STR("Thirdperson"), &hotkeys->thirdperson);
		}

		ImGui::NextColumn();

		if (settings->antiaim.enable) {
			ImGui::Text(STRC("| Anti-aim"));
			//	ImGui::Separator();
			{
				hotkey(STR("Manual right"), &hotkeys->manual_right);
				hotkey(STR("Manual left"), &hotkeys->manual_left);
				hotkey(STR("Manual back"), &hotkeys->manual_back);
				hotkey(STR("Manual forward"), &hotkeys->manual_forward);

				hotkey(STR("Desync inverter"), &hotkeys->desync_inverter);
				hotkey(STR("Slow walk"), &hotkeys->slow_walk);
				hotkey(STR("Freestand"), &hotkeys->freestand);
			}
		}
	}

	static void misc_tab() { 
		ImGui::Columns(2, nullptr, false);

		begin_child(STR("| Movement"), 285.f);
		{
			checkbox(STRC("Bunny-hop"), &settings->movement.bhop);
			checkbox(STRC("Auto-strafe"), &settings->movement.autostrafe);

			if (settings->movement.autostrafe)
				slider_int(STR("Strafe smoothness"), &settings->movement.strafe_smooth, 0, 200);

			checkbox(STRC("Fast stop"), &settings->movement.fast_stop);
			combo(STR("Legs movement"), &settings->movement.leg_movement, { STR("Avoid slide"), STR("Always slide"), STR("Break move") });

			ImGui::Spacing();
			ImGui::Text(STRC("Peek assist"));
			ImGui::SameLine();
			ImGui::ColorEdit4(STRC("##peek_assist_color_0"), settings->movement.peek_assist_colors[0].base(), ImGuiColorEditFlags_AlphaBar);
			ImGui::SameLine();
			ImGui::ColorEdit4(STRC("##peek_assist_color_1"), settings->movement.peek_assist_colors[1].base(), ImGuiColorEditFlags_AlphaBar);
			checkbox(STRC("Retreat on key release"), &settings->movement.peek_assist_retreat_on_key);
			ImGui::Separator();
		}
		end_child();

		begin_child(STR("Actions"));
		{
			const char* config_names[] = {
				"config_1",
				"config_2",
				"config_3",
				"config_4",
				"config_5",
				"config_6"
			};

			combo(STRC("Configs"), &settings->configs.config_number, { STR("Config 1"), STR("Config 2"), STR("Config 3"), STR("Config 4"), STR("Config 5"), STR("StopCode cfg") });

			if (ImGui::ButtonLS(STRC("Save"), ImVec2{ 65.f, 24.f }, 1)) {
				settings->save(STR(config_names[settings->configs.config_number]));
			}

			ImGui::SameLine();

			if (ImGui::ButtonLS(STRC("Load"), ImVec2{ 65.f, 24.f }, 2)) {
				settings->load(STR(config_names[settings->configs.config_number]));
			}

			combo(STRC("DPI Scale"), &settings->dpi_scale, { STR("100%"), STR("125%"), STR("150%"), STR("175%"), STR("200%") });
		}
		end_child();

		ImGui::NextColumn();

		begin_child(STR("| Other"));
		{
			checkbox(STRC("Hit sound"), &settings->misc.hitsound);
			if (settings->misc.hitsound) {
				combo(STR("Type"), &settings->misc.hitsound_type, { STR("Amethyst"), STR("Sparkle"), STR("Tap"), STR("Flick"), STR("Bepis"), STR("Arena switch"), STR("Warning") });
				slider_int(STR("Hit sound volume"), &settings->misc.hitsound_volume, 1, 100, STR("%d%%"));
			}

			checkbox(STRC("Unlock inventory"), &settings->misc.unlock_inventory);
			checkbox(STRC("Unlock hidden convars"), &settings->misc.unlock_cvars);

			checkbox(STRC("Preverse killfeed"), &settings->misc.preserve_killfeed);
			checkbox(STRC("Knife-bot"), &settings->misc.knife_bot);
			//checkbox(STRC("Shared esp"), &settings->misc.shared_esp);

			multicombo(STR("Cheat logs"), &settings->misc.log_filter, { STR("Aimbot info"), STR("Damage received"), STR("Damage dealt"), STR("Missed shots") });

#ifdef _DEBUG
			if (ImGui::Button(STRC("Unload"), ImVec2{ dpi::scale(56.f), dpi::scale(24.f) }))
				ctx->unload = true;
#endif

			checkbox(STRC("Auto-buy"), &settings->misc.autobuy.enable);

			combo(STR("Main weapon"), &settings->misc.autobuy.main, { STR("None"), STR("Auto sniper"), STR("Scout"), STR("AWP"), STR("Negev"), STR("M249"), STR("Rifle"), STR("AUG/SG553") });

			combo(STR("Pistol"), &settings->misc.autobuy.pistol, { STR("None"), STR("Dual berettas"), STR("P250"), STR("Five-Seven/Tec-9"), STR("Deagle/Revolver") });

			multicombo(STR("Additional"), &settings->misc.autobuy.additional, {
																					  STR("Head helmet"),  // 1
																					  STR("Other helmet"), // 2
																					  STR("HE grenade"),   // 4
																					  STR("Molotov"),	   // 8
																					  STR("Smoke"),		   // 16
																					  STR("Taser"),		   // 32
																					  STR("Defuse kit"),   // 64
																			  });
			ImGui::Separator();
		}

		end_child();
	}
    #ifdef _DEBUG
	static void lua_tab() {
		ImGui::Columns(2, nullptr, false);

		begin_child(STR("| Lua settings"), 285.f);
		{
			bool testtt = false;
			checkbox(STRC("Defensive flicks"), &testtt);

			checkbox(STRC("Air hitchance"), &testtt);
			//combo(STR("Legs movement"), &settings->movement.leg_movement, { STR("Avoid slide"), STR("Always slide"), STR("Break move") });

			ImGui::Spacing();
			//ImGui::Text(STRC("Peek assist"));
			//ImGui::SameLine();
			//ImGui::ColorEdit4(STRC("##peek_assist_color_0"), settings->movement.peek_assist_colors[0].base(), ImGuiColorEditFlags_AlphaBar);
			//ImGui::SameLine();
			//ImGui::ColorEdit4(STRC("##peek_assist_color_1"), settings->movement.peek_assist_colors[1].base(), ImGuiColorEditFlags_AlphaBar);
			//checkbox(STRC("Retreat on key release"), &settings->movement.peek_assist_retreat_on_key);
			//ImGui::Separator();
		}
		end_child();

		ImGui::NextColumn();
		begin_child(STR("| Lua list"));
		ImGui::Spacing();
		ImGui::Text("AstralQuo.su lua");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Button("Unload");
		ImGui::SameLine();
		ImGui::Button("Load");
		end_child();
	}
		#endif // _DEBUG
	//static void configs_tab() {
	/*ImGui::Columns(2, nullptr, false);

		const char* config_name[] = { "Config 1", "Config 2", "Config 3", "Config 4" };

		begin_child(STR("Actions"));
		{
			if (ImGui::Button(STRC("Save"), ImVec2{ dpi::scale(56.f), dpi::scale(24.f) })) {
				settings->save(STR(config_name[settings->configs.config_number]));
			}

			if (ImGui::Button(STRC("Load"), ImVec2{ dpi::scale(56.f), dpi::scale(24.f) })) {
				settings->load(STR(config_name[settings->configs.config_number]));
			}
		}
		end_child();

		ImGui::NextColumn();

		begin_child(STR("Config's list"));
		{
			combo(STRC("Configs"), &settings->configs.config_number, { STR("Config 1"), STR("Config 2"), STR("Config 3"), STR("Config 4") });
		}
		end_child();*/
	//}

	STFI void render_menu() {

		//for (size_t i = 0; i < tabs_list.size(); ++i) {
		//if (i == 0)
		//ImGui::SetCursorPosY(100.f + 25.f);

		//if (i > 0) {
		//ImGui::SetCursorPosY(100.f + (60.f * i));
		//ImGui::NewLine();
		//ImGui::Spacing();
		//}

		//ImGui::PushID(i);

		//if (i == (int)e_tabs::ragebot) {				
		//ImGui::Image(rage_icon, ImVec2(24, 24)); 
		//ImGui::SameLine();
		//}

		//if (ImGui::ButtonTabs(tabs_list[i].c_str(), ImVec2{ 110.f, 30.f }))
		//		current_tab = (e_tabs)i;

		//		ImGui::PopID();
		//		}

			ImGui::Columns(2, "tabs", false);

			ImGui::SetColumnWidth(0, 135.f);

			ImGui::BeginChild("tabs_", ImVec2(125.f, 580.f));
			{
				ImGui::SetCursorPosY(20.f);

				ImGui::Image(logo, ImVec2(115, 95)); //ImGui::Image(logo, ImVec2(105, 105));

				for (size_t i = 0; i < tabs_list.size(); ++i) {

					if (i == 0)
						ImGui::SetCursorPosY(100.f + 25.f);

					if (i > 0) {
						ImGui::SetCursorPosY(100.f + (60.f * i));
						ImGui::NewLine();
						ImGui::Spacing();
					}

					//ImGui::PushID(i);

				//	if (i == (int)e_tabs::ragebot) {
						//ImGui::SetCursorPosY();
					//	ImGui::Image(rage_icon, ImVec2(24, 24));
					//	ImGui::SameLine();
					//}

					if (ImGui::ButtonTabs(tabs_list[i].c_str(), ImVec2{ 120.f, 35.f }))
						current_tab = (e_tabs)i;

					ImGui::PopID();

				}

				ImGui::Spacing();
			}
			ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild("switch_tabs", ImVec2(580, 580), true, ImGuiWindowFlags_NoScrollbar);
			{
				switch (current_tab) {
					case e_tabs::ragebot: return ragebot_tab();
					case e_tabs::antiaim: return antiaim_tab();
					case e_tabs::players: return players_tab();
					case e_tabs::world: return world_tab();
					case e_tabs::hotkeys: return hotkeys_tab();
					case e_tabs::misc: return misc_tab();
					//case e_tabs::configs: return configs_tab();
					//case e_tabs::scripts: return;
					default: return welcome_tab();
				}
			}
			ImGui::EndChild();
		}
		__forceinline void render() {
			if (!open)
				return;

			CREATE_TEXTURE_MEM(edit_button, button_color_settings, sizeof(button_color_settings));
			CREATE_TEXTURE_MEM(astralquo_image, astralquo_logo, sizeof(astralquo_logo));
			CREATE_TEXTURE_MEM(ragebot_image, rage_icon, sizeof(rage_icon));

			logo = astralquo_image;
			button_image = edit_button;
			ImTextureID rage_icon_texture = ragebot_image;

		// ActivateGameOverlayToWebPage
		ImVec2 window_size = ImVec2{ 750.f, 625.f };
		ImVec2 screen_size = ImGui::GetIO().DisplaySize;
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding = ImVec2(20.f, 20.f); 
		style.WindowBorderSize = 0.f;
		style.WindowRounding = 7.0f;

		ImGui::StyleColorsDark();
		ImGui::PushFont(fonts::menu_desc);
		ImGui::SetNextWindowPos(ImVec2((screen_size.x - window_size.x) / 2.f, (screen_size.y - window_size.y) / 2.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(window_size, ImGuiCond_FirstUseEver);
		ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar);
		ImGui::Begin(dformat(STRS("AstralQuo.su | BETA | Updated: {} {}"), STR(__DATE__), STR(__TIME__)).c_str(), &open,
					 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollbar);
	
    // Получаем позицию и размер меню
		ImVec2 menu_pos = ImGui::GetWindowPos();
		ImVec2 menu_size = ImGui::GetWindowSize();
		ImVec2 menu_max = ImVec2(menu_pos.x + menu_size.x, menu_pos.y + menu_size.y);
		//float alpha = 9.05f; //0.2f
		//float blur_strength = 11.5f;
		

        shaders::create_blur(ImGui::GetBackgroundDrawList(),
							 ImVec2(menu_pos.x, menu_pos.y),
							 ImVec2(menu_max.x, menu_max.y));
		                              
		render_menu();

		ImGui::EndChild();
		ImGui::End();
		//shaders::create_blur(ImGui::GetBackgroundDrawList(), menu_size, menu_max, 0.5); //x.xf);
		ImGui::PopFont();
	}

} // namespace menu