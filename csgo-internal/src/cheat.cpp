#include "cheat.hpp"

//#include "../src/features/visuals/watermark.hpp"

#include "../deps/weave-gui/include.hpp"

#include "hooks/hooks.hpp"
#include "interfaces.hpp"

#include "game/events.hpp"
#include "game/netvar_manager.hpp"
#include "game/override_entity_list.hpp"

#include "utils/displacement.hpp"
#include "utils/encoding.hpp"
#include "utils/threading.hpp"

#include "features/bullets.hpp"
#include "features/discord.hpp"
#include "features/menu.hpp"
#include "features/network.hpp"
#include "features/visuals/chams.hpp"
#include "features/visuals/logs.hpp"
#include "features/visuals/skin_changer.hpp"

#include "lua/api.hpp"
#include "lua/script.hpp"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "urlmon")
#pragma comment(lib, "ws2_32")

using namespace std::chrono_literals;

namespace cheat {
	STFI bool is_dir(std::string dir) {
		DWORD flag = GetFileAttributesA(dir.c_str());
		if (flag == XOR32S(0xFFFFFFFFUL))
			if (GetLastError() == XOR32S(ERROR_FILE_NOT_FOUND))
				return false;

		if (!(flag & XOR32S(FILE_ATTRIBUTE_DIRECTORY)))
			return false;

		return true;
	}

	STFI void create_dir_if_not_exists(std::string dir) {
		if (!is_dir(dir))
			CreateDirectoryA(dir.c_str(), NULL);
	}

	STFI bool download_asset(std::string file_name, std::string path)
	{
		return URLDownloadToFileA(NULL, file_name.c_str(), path.c_str(), BINDF_GETNEWESTVERSION, NULL) == S_OK;
	}

#ifdef _DEBUG
	STFI void create_console() {
		AllocConsole();
		SetConsoleTitle(STRSC("csgo"));
		freopen_s((FILE**)stdout, STRSC("CONOUT$"), STRSC("w"), stdout);
	}

	STFI void destroy_console() {
		HWND console = GetConsoleWindow();

		fclose((FILE*)stdout);
		FreeConsole();

		PostMessage(console, XOR32S(WM_CLOSE), 0, 0);
	}
#endif

	CHEAT_INIT void init(LPVOID thread_parameter) {
#ifdef _DEBUG
		create_console();
#endif
		PUSH_LOG(STRSC("===========================\n"));
		PUSH_LOG(STRSC("new log started\n"));

		//if (!network::create_connection()) {
		//	PUSH_LOG(STRSC("failed to connect\n"));
		//	return;
		//}

		render::current_load_stage_name = STRS("Loading cheat..."); //setting up...

		//menu->window->set_size(gui::dpi::scale(menu->window_size));
		//menu->window->set_loading(STRS("Preparing..."));

		//gui::add_instance(menu->window);

		create_dir_if_not_exists(STRS("astralquo\\"));
		create_dir_if_not_exists(STRS("astralquo\\settings\\"));
		//create_dir_if_not_exists(STRS("astralquo\\lua\\"));

		create_dir_if_not_exists(STRS("C:\\astralquo"));
		create_dir_if_not_exists(STRS("C:\\astralquo\\assets"));
		create_dir_if_not_exists(STRS("C:\\astralquo\\fonts"));

		create_dir_if_not_exists(STRS("C:\\ProgramData\\astralquo"));
		create_dir_if_not_exists(STRS("C:\\ProgramData\\astralquo\\assets"));
		create_dir_if_not_exists(STRS("C:\\ProgramData\\astralquo\\fonts"));

		PUSH_LOG(STRSC("getting modules\n"));

		utils::wait_for_module(STRS("serverbrowser.dll"));

		PUSH_LOG(STRSC("downloading resources\n"));
		{
            //cfg
			if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/settings/config_1"), STRS("astralquo\\settings\\config_6")))
			    PUSH_LOG(STRSC("unable to download StopCode.cfg"));
			//Shared esp logos
			if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/fonts/logo_79x79.png"), STRS("csgo\\materials\\panorama\\images\\icons\\xp\\level4444.png")))
				PUSH_LOG(STRSC("unable to download logo_79x79\n"));

		 	if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/fonts/airflow_logo.png"), STRS("csgo\\materials\\panorama\\images\\icons\\xp\\level2512.png")))
		 		PUSH_LOG(STRSC("unable to download airflow_logo\n"));

		 	if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/fonts/logo_79x79.png"), STRS("csgo\\materials\\panorama\\images\\icons\\xp\\level44440.png")))
		 		PUSH_LOG(STRSC("unable to download logo_79x79rus\n"));

		 	if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/fonts/logo_79x79.png"), STRS("csgo\\materials\\panorama\\images\\icons\\xp\\level2001.png")))
		 		PUSH_LOG(STRSC("unable to download karnazity_logo\n"));

		 	if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/fonts/airflow_crack.png"), STRS("csgo\\materials\\panorama\\images\\icons\\xp\\level25120.png")))
		 		PUSH_LOG(STRSC("unable to download airflow_crack\n"));

			if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/lang_native.json"), STRS("C:\\astralquo\\keystocks.json")))
				PUSH_LOG(STRSC("unable to download lang_native\n"));

			//Fonts
			if (!download_asset(STRS("https://github.com/StopCode0notoff/astralquo/raw/refs/heads/main/fonts/fa-brands-400.ttf"), STRS("C:\\ProgramData\\astralquo\\fonts\\fa-brands-400.ttf")))
				PUSH_LOG(STRSC("unable to download fa-brands-400\n"));

			if (!download_asset(STRS("https://github.com/StopCode0notoff/astralquo/raw/refs/heads/main/fonts/fa-regular-400.ttf"), STRS("C:\\ProgramData\\astralquo\\fonts\\fa-regular-400.ttf")))
				PUSH_LOG(STRSC("unable to download fa-regular-400\n"));

			if (!download_asset(STRS("https://github.com/StopCode0notoff/astralquo/raw/refs/heads/main/fonts/fa-solid-900.ttf"), STRS("C:\\ProgramData\\astralquo\\fonts\\fa-solid-900.ttf")))
				PUSH_LOG(STRSC("unable to download fa-solid-900\n"));

			if (!download_asset(STRS("https://github.com/StopCode0notoff/astralquo/raw/refs/heads/main/fonts/RobotoFlex-Bold.ttf"), STRS("C:\\ProgramData\\astralquo\\fonts\\RobotoFlex-Bold.ttf")))
				PUSH_LOG(STRSC("unable to download RobotoFlex-Bold\n"));

			if (!download_asset(STRS("https://github.com/StopCode0notoff/astralquo/raw/refs/heads/main/fonts/RobotoFlex-Regular.ttf"), STRS("C:\\ProgramData\\astralquo\\fonts\\RobotoFlex-Regular.ttf")))
				PUSH_LOG(STRSC("unable to download RobotoFlex-Regular\n"));

			if (!download_asset(STRS("https://github.com/StopCode0notoff/astralquo/raw/refs/heads/main/fonts/RobotoFlex-SemiBold.ttf"), STRS("C:\\ProgramData\\astralquo\\fonts\\RobotoFlex-SemiBold.ttf")))
				PUSH_LOG(STRSC("unable to download RobotoFlex-SemiBold\n"));

			if (!download_asset(STRS("https://github.com/StopCode0notoff/astralquo/raw/refs/heads/main/fonts/Root-UI-Bold.ttf"), STRS("C:\\ProgramData\\astralquo\\fonts\\Root-UI-Bold.ttf")))
				PUSH_LOG(STRSC("unable to download Root-UI-Bold\n"));

			if (!download_asset(STRS("https://github.com/StopCode0notoff/astralquo/raw/refs/heads/main/fonts/Smallest-Pixel7.ttf"), STRS("C:\\ProgramData\\astralquo\\fonts\\Smallest-Pixel7.ttf")))
				PUSH_LOG(STRSC("unable to download Smallest-Pixel7\n"));

			//Images
			if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/assets/error.png"), STRS("C:\\ProgramData\\astralquo\\assets\\error.png")))
				PUSH_LOG(STRSC("unable to download error(image)\n"));

			if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/assets/hit.png"), STRS("C:\\ProgramData\\astralquo\\assets\\hit.png")))
				PUSH_LOG(STRSC("unable to download hit(image)\n"));

			if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/assets/hotkeys_icon.png"), STRS("C:\\ProgramData\\astralquo\\assets\\hotkeys_icon.png")))
				PUSH_LOG(STRSC("unable to download hotkeys_icon(image)\n"));

			if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/assets/info.png"), STRS("C:\\ProgramData\\astralquo\\assets\\info.png")))
				PUSH_LOG(STRSC("unable to download info(image)\n"));

			if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/assets/miss.png"), STRS("C:\\ProgramData\\astralquo\\assets\\miss.png")))
				PUSH_LOG(STRSC("unable to download miss(image)\n"));

			if (!download_asset(STRS("https://raw.githubusercontent.com/StopCode0notoff/astralquo/refs/heads/main/assets/settings_icon.png"), STRS("C:\\ProgramData\\astralquo\\assets\\settings_icon.png")))
				PUSH_LOG(STRSC("unable to download settings_icon(image)\n"));
		 }

		PUSH_LOG(STRSC("resources downloaded\n"));

		if (!network::setup()) {
			PUSH_LOG(STRSC("network error\n"));
			exit(0);
		}

		PUSH_LOG(STRSC("network passed\n"));

		patterns::init();
		PUSH_LOG(STRSC("patterns passed\n"));
		interfaces::init();
		PUSH_LOG(STRSC("interfaces passed\n"));
		netvars->init();
		PUSH_LOG(STRSC("netvars passed\n"));
		hooks::init();
		PUSH_LOG(STRSC("hooks passed\n"));

		esp::chams->init();
		PUSH_LOG(STRSC("chams passed\n"));
		entities->init();
		PUSH_LOG(STRSC("entity list passed\n"));
		default_settings->init();
		settings->init();
		PUSH_LOG(STRSC("settings passed\n"));
		game_events->init();
		PUSH_LOG(STRSC("game events passed\n"));

		// akatsuki's dumb promise
		if (auto cvar = GET_CVAR("zoom_sensitivity_ratio_mouse"); cvar != nullptr)
			cvar->set_value(0.f);

		if (!threading->init()) {
			PUSH_LOG(STRSC("threading error\n"));
			exit(0);
			return;
		}
		PUSH_LOG(STRSC("threading passed\n"));

		gui::resources::add_pngs(network::get_images());
		gui::resources::downloaded = true;
		render::can_render = true;

		PUSH_LOG(STRSC("resources downloaded\n"));
		//menu->window->stop_loading();
		user_settings::load("settings");
		PUSH_LOG(STRSC("injected\n"));

		constexpr std::pair<color_t, color_t> colors = {
			{ 0, 51, 255 },
			{ 255, 59, 80 }
		};

		const auto astralquo_logo = STRS(R"(
               AAA                                       tttt                                             lllllll      QQQQQQQQQ                                        
              A:::A                                   ttt:::t                                             l:::::l    QQ:::::::::QQ                                      
             A:::::A                                  t:::::t                                             l:::::l  QQ:::::::::::::QQ                                    
            A:::::::A                                 t:::::t                                             l:::::l Q:::::::QQQ:::::::Q                                   
           A:::::::::A             ssssssssss   ttttttt:::::ttttttt   rrrrr   rrrrrrrrr   aaaaaaaaaaaaa    l::::l Q::::::O   Q::::::Quuuuuu    uuuuuu     ooooooooooo   
          A:::::A:::::A          ss::::::::::s  t:::::::::::::::::t   r::::rrr:::::::::r  a::::::::::::a   l::::l Q:::::O     Q:::::Qu::::u    u::::u   oo:::::::::::oo 
         A:::::A A:::::A       ss:::::::::::::s t:::::::::::::::::t   r:::::::::::::::::r aaaaaaaaa:::::a  l::::l Q:::::O     Q:::::Qu::::u    u::::u  o:::::::::::::::o
        A:::::A   A:::::A      s::::::ssss:::::stttttt:::::::tttttt   rr::::::rrrrr::::::r         a::::a  l::::l Q:::::O     Q:::::Qu::::u    u::::u  o:::::ooooo:::::o
       A:::::A     A:::::A      s:::::s  ssssss       t:::::t          r:::::r     r:::::r  aaaaaaa:::::a  l::::l Q:::::O     Q:::::Qu::::u    u::::u  o::::o     o::::o
      A:::::AAAAAAAAA:::::A       s::::::s            t:::::t          r:::::r     rrrrrrraa::::::::::::a  l::::l Q:::::O     Q:::::Qu::::u    u::::u  o::::o     o::::o
     A:::::::::::::::::::::A         s::::::s         t:::::t          r:::::r           a::::aaaa::::::a  l::::l Q:::::O  QQQQ:::::Qu::::u    u::::u  o::::o     o::::o
    A:::::AAAAAAAAAAAAA:::::A  ssssss   s:::::s       t:::::t    ttttttr:::::r          a::::a    a:::::a  l::::l Q::::::O Q::::::::Qu:::::uuuu:::::u  o::::o     o::::o
   A:::::A             A:::::A s:::::ssss::::::s      t::::::tttt:::::tr:::::r          a::::a    a:::::a l::::::lQ:::::::QQ::::::::Qu:::::::::::::::uuo:::::ooooo:::::o
  A:::::A               A:::::As::::::::::::::s       tt::::::::::::::tr:::::r          a:::::aaaa::::::a l::::::l QQ::::::::::::::Q  u:::::::::::::::uo:::::::::::::::o
 A:::::A                 A:::::As:::::::::::ss          tt:::::::::::ttr:::::r           a::::::::::aa:::al::::::l   QQ:::::::::::Q    uu::::::::uu:::u oo:::::::::::oo 
AAAAAAA                   AAAAAAAsssssssssss              ttttttttttt  rrrrrrr            aaaaaaaaaa  aaaallllllll     QQQQQQQQ::::QQ    uuuuuuuu  uuuu   ooooooooooo   
                                                                                                                               Q:::::Q                                  
                                                                                                                                QQQQQQ                                  
                                                                                                                                                                        
                                                                                                                                                                        )");

		game_console->clear();
		std::this_thread::sleep_for(200ms);
		game_console->print_colored_id(STR("\n\n\n"));
		game_console->print_gradient_text(astralquo_logo, colors);
		game_console->print_colored_id(STR("\n\n\n"));

		skin_changer->parse();
		PUSH_LOG(STRSC("A HY CDALI DNEVNIKI 4TOBI 9l POSTAVILA TWO BALS\n"));
	}

	CHEAT_INIT void unload() {
		hooks::unload();
		entities->remove();
		threading->remove();

#ifdef _DEBUG
		destroy_console();
#endif
	}

	void debug_log(const char* message, ...) {
		static std::mutex mutex{};
		THREAD_SAFE(mutex);

		FILE* file = fopen(STRSC("C:\\astralquo\\log.txt"), STRSC("ab"));
		_fseeki64(file, 0, SEEK_END);

		char output[4096] = {};
		va_list args;
		va_start(args, message);
		vsprintf_s(output, message, args);
		va_end(args);

		fwrite(output, 1, strlen(output), file);
		fclose(file);
	}
} // namespace cheat
