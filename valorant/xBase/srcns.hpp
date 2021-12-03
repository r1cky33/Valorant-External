#pragma once

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

namespace srcns {
#ifndef _CHEAT
#define _CHEAT
	struct LocalPlayer {
		uintptr_t	local_pawn{};
		uintptr_t	player_state{};

		int32_t	team{};

		float		fov{};
		Vector3		camera_position{};
		Vector3		camera_rotation{};

		void get_camera() {
			magic::HijackState state = magic::read_results();
			this->camera_position = state.camera.position;
			this->camera_rotation = state.camera.rotation;

			this->fov = state.camera.fov;
		}

		void get_localplayer(magic::HijackState& state) {
			this->local_pawn = state.localpawn;
			this->player_state = state.playerstate;
			this->team = core::read<int32_t>(globals::target_proc::target_process_id, core::read<uint64_t>(globals::target_proc::target_process_id, this->player_state + offsets::actor::playerstate::team_id_dref) + offsets::actor::playerstate::team_id);
		}
	};

	extern LocalPlayer localPlayer;

//#pragma pack(push, 1)
//	struct entityCache {		// 0x30 bytes
//		uint32_t unique_id;		//4 bytes	
//		uint32_t bdormant;		//4 bytes
//		uint64_t playerstate;	//8 bytes
//		uint64_t mesh;			//8 bytes
//		uint64_t root_comp;		//8 bytes
//		uint64_t dmg_ctrl;		//8 bytes
//		uint64_t pobjptr;		//8 bytes
//	};
//#pragma pack(pop)

#pragma pack(push, 1)
	struct entityCache {			// 0x34 bytes
		uint32_t unique_id;			//	4 bytes    
		uint32_t bdormant;			//	4 bytes 
		float    health;           //	4 bytes   
		uint32_t isalive;			//	4 bytes 
		uint64_t playerstate;		//	8 bytes  
		uint64_t mesh;				//	8 bytes
		uint64_t root_comp;			//	8 bytes
		uint64_t pobjptr;			//	8 bytes
		uint32_t bonecount;			//	4 bytes
	};
#pragma pack(pop)

	struct TslEntity {
		uintptr_t	p_obj_ptr{};
		uintptr_t	root_comp{};
		uintptr_t	damage_ctrl{};
		uintptr_t	player_state{};
		uintptr_t	mesh{};
		uintptr_t	skeletal_mesh{};

		uint32_t	unique_id{};
		float		health{};
		bool		b_dormant{};
		bool		b_recently_rendered{};
		bool		b_alive{};
		int32_t		team{};
		uint32_t	num_bones{};

		Vector3		root_position{};
		Vector3		head_position{};

		Vector3		root_position_2d{};
		Vector3		head_position_2d{};

	private:
		void get_team(uintptr_t player_state) {
			if (!utils::is_valid_addr(player_state)) { return; }
			this->player_state = player_state;
			this->team = core::read<int32_t>(globals::target_proc::target_process_id, core::read<uint64_t>(globals::target_proc::target_process_id, player_state + offsets::actor::playerstate::team_id_dref) + offsets::actor::playerstate::team_id);
		}

		void get_mesh(uintptr_t mesh) {
			this->mesh = mesh;

			this->skeletal_mesh = core::read<uintptr_t>(globals::target_proc::target_process_id, mesh + offsets::actor::mesh::skeletal_mesh);
			this->num_bones = core::read<uint32_t>(globals::target_proc::target_process_id, this->skeletal_mesh + offsets::actor::mesh::bones_num);
		}

		void get_visibility(uintptr_t mesh) {
			byte b_recently_rendered = core::read<byte>(globals::target_proc::target_process_id, mesh + offsets::actor::mesh::b_recently_rendered);
			this->b_recently_rendered = utils::IsBitSet(b_recently_rendered, 5) ? true : false;
		}

		void get_root_position(uintptr_t root_comp) {
			this->root_comp = root_comp;
			this->root_position = core::read<Vector3>(globals::target_proc::target_process_id, this->root_comp + offsets::actor::root_pos);
		}

		void get_health(uintptr_t damage_ctrl) {
			this->damage_ctrl = damage_ctrl;
			this->health = core::read<float>(globals::target_proc::target_process_id, damage_ctrl + offsets::actor::dmg_ctrl::health);
			this->b_alive = core::read<bool>(globals::target_proc::target_process_id, damage_ctrl + offsets::actor::dmg_ctrl::is_alive);

			if (this->health > 100)
				this->health = 100;
		}

		void get_dormant(uint32_t bdormant) {
			//get the first byte the 1337 ghetto way
			this->b_dormant = (bool)(bdormant & ((1 << 8) - 1));
		}
		
	public:
		void get_2d_pos() {
			this->head_position = engine::GetBoneWithRotation(this->mesh, engine::e_male_bones::Head);
			this->root_position = core::read<Vector3>(globals::target_proc::target_process_id, root_comp + (uintptr_t)offsets::actor::root_pos);

			this->head_position_2d = engine::WorldToScreen(this->head_position, localPlayer.camera_position, localPlayer.camera_rotation, localPlayer.fov);
			this->root_position_2d = engine::WorldToScreen(this->root_position, localPlayer.camera_position, localPlayer.camera_rotation, localPlayer.fov);
		}

		//bool get_info(entityCache& cached) {
		//	if (cached.unique_id != 0x11e0101) {
		//		return false;
		//	}

		//	this->p_obj_ptr = cached.pobjptr;
		//	this->unique_id = cached.unique_id;

		//	if (!utils::is_valid_addr(cached.mesh) || !utils::is_valid_addr(cached.root_comp) /*|| !utils::is_valid_addr(cached.dmg_ctrl)|| !utils::is_valid_addr(cached.playerstate)*/) {
		//		return false;
		//	}

		//	this->get_dormant(cached.bdormant);
		//	this->get_health(cached.dmg_ctrl);
		//	if (this->b_dormant || this->health < 1) { return false; }

		//	//mesh related
		//	this->get_mesh(cached.mesh);

		//	this->get_visibility(cached.mesh);
		//	this->get_root_position(cached.root_comp);

		//	this->get_2d_pos();
		//	this->get_team(cached.playerstate);

		//	return true;
		//}

		//bool get_info(entityCache& cached) {
		//	if (cached.unique_id != 0x11e0101) {
		//		return false;
		//	}

		//	this->p_obj_ptr = cached.pobjptr;
		//	this->unique_id = cached.unique_id;

		//	this->get_dormant(cached.bdormant);
		//	//this->get_health(cached.dmg_ctrl);

		//	//mesh related
		//	//this->get_mesh(cached.mesh);

		//	this->get_visibility(cached.mesh);
		//	this->get_root_position(cached.root_comp);

		//	this->get_2d_pos();
		//	this->get_team(cached.playerstate);

		//	return true;
		//}

		bool get_info(entityCache& cached) {
			if (cached.unique_id != 0x11e0101) {
				return false;
			}

			this->p_obj_ptr = cached.pobjptr;
			this->unique_id = cached.unique_id;

			this->health = cached.health;
			this->b_alive = cached.isalive;

			this->b_dormant = cached.bdormant;
			this->num_bones = cached.bonecount;
			//mesh related
			//this->get_mesh(cached.mesh);
			this->mesh = cached.mesh;

			if (this->b_dormant || this->health < 1) { return false; }

			//this->get_visibility(cached.mesh);
			this->get_root_position(cached.root_comp);

			this->get_2d_pos();
			this->get_team(cached.playerstate);

			return true;
		}
	};
#endif

	extern bool init();
}
