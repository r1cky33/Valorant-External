#include "stdafx.hpp"

namespace offsets {
	namespace ginstance {

		const uint32_t localplayer_array = 0x40;

		namespace localplayer {
			const uint32_t local_player = 0x0;
			const uint32_t local_ctrl = 0x38;
			const uint32_t local_pawn = 0x438;
			const uint32_t local_root = 0x238;
			const uint32_t local_position = 0x184;

			const uint32_t l_camera_manager = 0x450;
			namespace camera_manager {
				const uint32_t camera_position = 0x11D0;
				const uint32_t camera_rotation = 0x11DC;
				const uint32_t camera_fov = 0x11E8;
			}
		}
	}

	namespace persistentlvl {
		const uint32_t actors_array = 0xa0;
		const uint32_t actors_count = 0xa8;
	}

	namespace actor {
		const uint32_t unique_id = 0x3C;
		const uint32_t id = 0x18;
		const uint32_t b_dormant = 0x100;

		const uint32_t root_comp = 0x210;
		// 0x15C
		const uint32_t root_pos = 0x164;

		const uint32_t a_mesh = 0x408;
		namespace mesh {
			const uint32_t skeletal_mesh = 0x538;
			const uint32_t bones_num = 0x1c0;
			const uint32_t b_recently_rendered = 0x69f;
		}

		const uint32_t a_dmg_ctrl = 0x978;
		namespace dmg_ctrl {
			const uint32_t is_alive = 0x1a9;
			const uint32_t health = 0x1b0;
		}

		const uint32_t a_playerstate = 0x3d0;
		namespace playerstate {
			const uint32_t team_id_dref = 0x580;
			const uint32_t team_id = 0xF8;
		}
	}
}

namespace cache {
	uintptr_t actors{};
	uint32_t actor_count{};
}