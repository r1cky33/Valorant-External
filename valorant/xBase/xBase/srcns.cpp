#include "stdafx.hpp"
#include "comp_render.hpp"

namespace srcns {
	bool init() {
		while (!globals::target_proc::target_process_base)
			globals::target_proc::target_process_base = core::get_process_base_by_id(globals::target_proc::target_process_id);

		::printf("[+] process id: %d	process base: 0x%p", globals::target_proc::target_process_id, (void*)globals::target_proc::target_process_base);

		d2d_window_t window{ };
		d2d_renderer_t renderer{ window._handle, globals::target_proc::target_wnd_hwnd };

		while(true)
		{
			renderer.begin_scene();
			
			renderer.draw_line(0, 100, 100, 0, 3, D2D1::ColorF::Red);

			renderer.end_scene();
		}

		return true;
	}
}