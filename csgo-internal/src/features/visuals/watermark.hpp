#pragma once
#include "../../base_includes.hpp"
#include "../../features/menu.hpp"

struct watermark_t {
	gui::instance::window_t* m_window{};
	void render();
};

GLOBAL_DYNPTR(watermark_t, watermark);