#pragma once

//namespace globals {
//	extern HWND l_hwnd;
//	extern HWND t_hwnd;
//
//	extern uint64_t t_process_base;
//	extern uint32_t t_proc_id;
//
//	extern uint32_t screen_width;
//	extern uint32_t screen_height;
//}

namespace offsets {
	namespace ginstance {

		extern const uint32_t localplayer_array;

		namespace localplayer {
			extern const uint32_t localplayer;
			extern const uint32_t local_ctrl;
			extern const uint32_t local_pawn;
			extern const uint32_t local_root;
			extern const uint32_t local_position;

			extern const uint32_t l_camera_manager;
			namespace camera_manager {
				extern const uint32_t camera_position;
				extern const uint32_t camera_rotation;
				extern const uint32_t camera_fov;
			}
		}
	}

	namespace persistentlvl {
		extern const uint32_t actors_array;
		extern const uint32_t actors_count;
	}

	namespace actor {
		extern const uint32_t unique_id;
		extern const uint32_t id;
		extern const uint32_t b_dormant;

		extern const uint32_t root_comp;
		extern const uint32_t root_pos;

		extern const uint32_t a_mesh;
		namespace mesh {
			extern const uint32_t bone_array;
			extern const uint32_t skeletal_mesh;
			extern const uint32_t bones_num;
			extern const uint32_t b_recently_rendered;
		}

		extern const uint32_t a_dmg_ctrl;
		namespace dmg_ctrl {
			extern const uint32_t is_alive;
			extern const uint32_t health;
		}

		extern const uint32_t a_playerstate;
		namespace playerstate {
			extern const uint32_t team_id_dref;
			extern const uint32_t team_id;
		}
	}
}

namespace cache {
	extern uintptr_t actors;
	extern uint32_t actor_count;
}