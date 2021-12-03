#include "stdafx.hpp"

namespace globals {
	namespace target_proc {
		uint64_t	target_process_base{};
		uint32_t	target_process_id{};
		HWND		target_wnd_hwnd{};
	}

	namespace wnd {
		float screen_res_width{};
		float screen_res_height{};
	}
}