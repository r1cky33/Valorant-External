#include "stdafx.hpp"

namespace settings {
	bool team = true;
	bool skeleton = true;
	bool head = true;
	bool box = true;
	bool health = true;
}

namespace nav {
	bool esp = false;
}

bool aimwin = false;
bool panic = false;

namespace srcns {
	std::vector<TslEntity> entityList;
	magic::HijackState state = { 0 };

	LocalPlayer localPlayer = { 0 };
	entityCache entities[0x1600] = { 0 };

	bool bShellcodeWritten = false;
	uintptr_t plocalproxy = 0;
	uintptr_t pentitycache = 0;
	uintptr_t decrypted_world = 0;

	bool menu = false;

	bool cache() {
		std::vector<TslEntity> tmpList;

		//do magic
		if (!bShellcodeWritten) {
			//really critical... will close the cheat on failure
			if (magic::magic_scan(decryptor::pkey, decryptor::pstate)) {
				Beep(300, 100); Beep(500, 100);
				decrypted_world = decryptor::read_uworld(globals::target_proc::target_process_base);
				if (!utils::is_valid_addr(decrypted_world)) { return false; }
				magic::write_shell(decrypted_world, globals::target_proc::target_process_base, pentitycache, plocalproxy);
			}
			else {
				MessageBoxA(nullptr, _xor_("AutoUpdate ERROR!").c_str(), "ERROR", NULL);
				exit(0);
			}
		}
		bShellcodeWritten = true;

		state = magic::read_results();

		cache::actors = state.actors.Ptr;
		cache::actor_count = state.actors.Size;

		if (cache::actor_count > 0x1600) { return false; }

		if (!utils::is_valid_addr(cache::actors)) { return false; }
		localPlayer.get_localplayer(state);

		//core::mem_cpy(globals::target_proc::target_process_id, pentitycache, GetCurrentProcessId(), (uintptr_t)entities, sizeof(entityCache) * cache::actor_count);

		localPlayer.get_camera();

		::printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		::printf(">	Camera.Rotation: x:	%.3f	y:	%.3f	z:	%.3f\n", localPlayer.camera_rotation.x, localPlayer.camera_rotation.y, localPlayer.camera_rotation.z);
		::printf(">	Camera.Position: x:	%.3f	y:	%.3f	z:	%.3f\n", localPlayer.camera_position.x, localPlayer.camera_position.y, localPlayer.camera_position.z);
		::printf("> Camera.Fov: %.3f\n", localPlayer.fov);
		::printf("> Actors Pointer: 0x%p\n", state.actors.Ptr);
		::printf("> Actors Count: 0x%x\n", state.actors.Size);
		::printf("> UWorld: 0x%p\n", decrypted_world);

		TslEntity tslEntity{};
		for (uint32_t i = 0; i < cache::actor_count; i++) {
			//entityCache cached = entities[i];

			entityCache cached = core::read<entityCache>(globals::target_proc::target_process_id, pentitycache + (sizeof(entityCache) * i));

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			if (cached.unique_id == 0x11e0101) {
				SetConsoleTextAttribute(hConsole, 4);
				::printf(" [+] Actor\n");
				::printf("		>	bIsAlive:  %s\n", cached.isalive ? "true" : "false");
				::printf("		>	health: %d\n", cached.health);
				::printf("		>	mesh: 0x%p\n", cached.mesh);
				::printf("		>	playerstate: 0x%p\n", cached.playerstate);
				::printf("		>	pobjptr: 0x%p\n", cached.pobjptr);
				::printf("		>	root_comp: 0x%p\n", cached.root_comp);
				::printf("		>	unique_id: 0x%x\n", cached.unique_id);
				::printf("		>	bdormant: %s\n", cached.bdormant ? "true" : "false");
				::printf("		>	bonecount: 0x%x\n", cached.bonecount);
				SetConsoleTextAttribute(hConsole, 1);
			}

			if (!tslEntity.get_info(cached)) {
				//SetConsoleTextAttribute(hConsole, 4);
				//::printf(" [+]	Invalid Actor\n");
				//::printf("		>	Mesh: 0x%p\n", tslEntity.mesh);
				//::printf("		>	Skeletal: 0x%p\n", tslEntity.skeletal_mesh);
				//::printf("		>	DamageHandler: 0x%p\n", tslEntity.damage_ctrl);
				//::printf("		>	Health: %d\n", tslEntity.health);
				//::printf("		>	PlayerState: 0x%p\n", tslEntity.player_state);
				//::printf("		>	RootComponent: 0x%p\n", tslEntity.root_comp);
				//::printf("		>	RootPosition:	x: %.3f		y:	%.3f	z:	%.3f\n", tslEntity.root_position.x, tslEntity.root_position.y, tslEntity.root_position.z);
				//::printf("		>	BoneCount: 0x%p\n", tslEntity.num_bones);
				//::printf("		>	bDormant: %s\n", tslEntity.b_dormant ? "true" : "false");
				//::printf("		>	HeadPosition: x: %.3f		y:	%.3f	z:	%.3f\n", tslEntity.head_position.x, tslEntity.head_position.y, tslEntity.head_position.z);
				//::printf("		>	UniqueId: 0x%p\n", tslEntity.unique_id);
				//::printf("		>	Team: 0x%p\n", tslEntity.team);
				//::printf("		>	bIsAlive:  %s\n", tslEntity.b_alive ? "true" : "false");
				continue;
			}
			else {
				//SetConsoleTextAttribute(hConsole, 2);
				////::printf(" [+]	Valid Actor\n");
				////::printf("		>	Mesh: 0x%p\n", tslEntity.mesh);
				////::printf("		>	Skeletal: 0x%p\n", tslEntity.skeletal_mesh);
				////::printf("		>	DamageHandler: 0x%p\n", tslEntity.damage_ctrl);
				////::printf("		>	Health: %d\n", tslEntity.health);
				////::printf("		>	PlayerState: 0x%p\n", tslEntity.player_state);
				////::printf("		>	RootComponent: 0x%p\n", tslEntity.root_comp);
				////::printf("		>	RootPosition:	x: %.3f		y:	%.3f	z:	%.3f\n", tslEntity.root_position.x, tslEntity.root_position.y, tslEntity.root_position.z);
				////::printf("		>	BoneCount: 0x%p\n", tslEntity.num_bones);
				////::printf("		>	bDormant: %s\n", tslEntity.b_dormant ? "true" : "false");
				////::printf("		>	HeadPosition: x: %.3f		y:	%.3f	z:	%.3f\n", tslEntity.head_position.x, tslEntity.head_position.y, tslEntity.head_position.z);
				////::printf("		>	UniqueId: 0x%p\n", tslEntity.unique_id);
				////::printf("		>	Team: 0x%p\n", tslEntity.team);
				////::printf("		>	bIsAlive:  %s\n", tslEntity.b_alive ? "true" : "false");
				tmpList.push_back(tslEntity);
			}
		}

		entityList = tmpList;
		return true;
	}

	void draw_skeleton(TslEntity& entity, uint32_t color) {
		if (entity.num_bones < 90) { return; }

		Vector3 neckpos = engine::GetBoneWithRotation(entity.mesh, engine::e_male_bones::Spine4);
		Vector3 pelvispos = engine::GetBoneWithRotation(entity.mesh, 72);

		//dxcomp::draw_circle(ImVec2{ entity.head_position_2d.x, entity.head_position_2d.y }, 
		//	(4500 / localPlayer.camera_position.Distance(entity.head_position)) * 2.5,
		//	color,
		//	2, 
		//	10);

		Vector3 previous(0, 0, 0);
		Vector3 current, p1, c1;

		for (auto a : (entity.num_bones == 99 ? engine::f_skeleton : engine::m_skeleton)) {
			previous = Vector3(0, 0, 0);

			for (int bone : a)
			{
				current = bone == 6 ? neckpos : (bone == 72 ? pelvispos : engine::GetBoneWithRotation(entity.mesh, bone));

				if (previous.x == 0.f)
				{
					previous = current;
					continue;
				}

				p1 = engine::WorldToScreen(previous, localPlayer.camera_position, localPlayer.camera_rotation, localPlayer.fov);
				c1 = engine::WorldToScreen(current, localPlayer.camera_position, localPlayer.camera_rotation, localPlayer.fov);

				dxcomp::draw_line(ImVec2{ p1.x, p1.y }, ImVec2{ c1.x, c1.y }, color, 2);

				previous = current;
			}
		}
	}

	void iteration() {
		if (!cache() || entityList.empty()) {
			return;
		}

		auto entityListCpy = entityList;

		float distance = 0.f;
		int revise = 0;
		float min_angle = FLT_MAX;
		TslEntity closes_entity{};

		for (uint32_t i = 0; i < entityListCpy.size(); i++) {
			TslEntity current = entityListCpy[i];

			if (localPlayer.local_pawn == current.p_obj_ptr || !utils::is_valid_addr(current.mesh)/*!utils::is_valid_addr(current.skeletal_mesh)*/)
				continue;

			//if (current.head_position.z <= current.root_position.z || current.head_position_2d.y > current.root_position_2d.y)
			//	continue;

			if (current.num_bones > 5 && current.num_bones < 40) {
				dxcomp::draw_corner_box(current.root_position_2d.x - 20.f, current.root_position_2d.y - 20.f, 100 / (distance / 3.5f), 100 / (distance / 3.5f), 2, true);
				continue;
			}

			if (current.num_bones < 99 || current.num_bones > 103) { continue; }
			if (!current.b_alive) { continue; }

			////{
			////	Vector3 neckpos = engine::GetBoneWithRotation(current.mesh, engine::e_male_bones::Spine4);
			////	Vector3 pelvispos = engine::GetBoneWithRotation(current.mesh, 72);

			////	if (neckpos.x < current.root_position.x - 200 || neckpos.x > current.root_position.x + 200 || neckpos.y < current.root_position.y - 200 || neckpos.y > current.root_position.y + 200 || neckpos.z < current.root_position.z - 200 || neckpos.z > current.root_position.z + 200)
			////		continue;

			////	if (pelvispos.x < current.root_position.x - 200 || pelvispos.x > current.root_position.x + 200 || pelvispos.y < current.root_position.y - 200 || pelvispos.y > current.root_position.y + 200 || pelvispos.z > current.root_position.z + 200 || pelvispos.z < current.root_position.z - 200)
			////		continue;
			////}

			localPlayer.get_camera();
			distance = localPlayer.camera_position.Distance(current.root_position);

			//int height = (current.root_position_2d.y - current.head_position_2d.y) * 2.5;

			float height = (2000 / distance) * (1920 / 1920) * 65;
			float width = height / 2;
			float x = current.root_position_2d.x - width / 2;
			float y = current.root_position_2d.y - height * 0.5f;

			if (!panic) {
				if (localPlayer.team == current.team) {
					if (settings::team) {
						//if (settings::head) {
						//	dxcomp::draw_circle(ImVec2{ current.head_position_2d.x, current.head_position_2d.y },
						//		(4500 / localPlayer.camera_position.Distance(current.head_position)) * 2.5,
						//		ImColor(0, 255, 0),
						//		1,
						//		10);
						//}

						if (settings::box) {
							dxcomp::draw_corner_box(x/*current.head_position_2d.x - height / 4*/, y/*current.head_position_2d.y - height / 6*/, width/*height / 2*/, height/*height*/, 1, true);
						}

						if (settings::health) {
							dxcomp::draw_health_bar(current.health, x, y, height, 0xff0aa0aa);
						}

						//if (settings::skeleton) {
						//	//ImGui::ColorConvertFloat4ToU32(ImVec4(0.51f, 0.36f, 0.15f, 1.00f))
						//	draw_skeleton(current, ImColor(0, 255, 0));
						//}
					}
				}
				else{
					//if (settings::head) {
					//	dxcomp::draw_circle(ImVec2{ current.head_position_2d.x, current.head_position_2d.y },
					//		(4500 / localPlayer.camera_position.Distance(current.head_position)) * 2.5,
					//		ImColor(0, 0, 255),
					//		1,
					//		10);
					//}

					if (settings::box) {
						dxcomp::draw_corner_box(x/*current.head_position_2d.x - height / 4*/, y/*current.head_position_2d.y - height / 6*/, width/*height / 2*/, height/*height*/, 1, false);
					}

					if (settings::health) {
						dxcomp::draw_health_bar(current.health, x, y, height, 0xff0aa0aa);
					}

					//if (settings::skeleton) {
					//	draw_skeleton(current, ImColor(0, 0, 255));
					//}
				}
			}
		}
	}

	bool init() {
		while (!globals::target_proc::target_process_base)
			globals::target_proc::target_process_base = core::get_process_base_by_id(globals::target_proc::target_process_id);

		::printf(_xor_("[#] process id: %d	process base: 0x%p").c_str(), globals::target_proc::target_process_id, (void*)globals::target_proc::target_process_base);
		//::ShowWindow(GetConsoleWindow(), SW_HIDE);

		bool show_demo_window = true;

		dxcomp::init_rendering();

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT)
		{
			if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
				continue;
			}

			globals::wnd::screen_res_height = ::GetSystemMetrics(SM_CYSCREEN);
			globals::wnd::screen_res_width = ::GetSystemMetrics(SM_CXSCREEN);

			{
				dxcomp::begin_scene();
				dxcomp::handle_input();

				if (menu) {
					ImGuiStyle& style = ImGui::GetStyle();

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.f, 18.f });
					ImGui::BeginMainMenuBar();
					ImGui::PopStyleVar();

					// navbar
					{
						style.Colors[ImGuiCol_Button] = nav::esp == true ? ImColor(199, 79, 13) : ImColor(18, 18, 18, 255);
						if (ImGui::Button(_xor_("Visuals").c_str(), ImVec2(150, 45))) { nav::esp == true ? nav::esp = false : nav::esp = true; };

						ImGui::SameLine(ImGui::GetWindowWidth() - 200);
						ImGui::BulletText("r1cky33");
					}
					ImGui::EndMainMenuBar();

					{
						if (nav::esp) {
							ImGui::Begin(_xor_("Visuals").c_str(), &aimwin, ImGuiWindowFlags_AlwaysAutoResize);

							ImGui::BeginChild(_xor_("ESP").c_str(), ImVec2{ 455.f,200.0f }, true);
							{
								ImGui::Columns(2);

								ImGui::NewLine();
								ImGui::Checkbox(_xor_("Team ESP ").c_str(), &settings::team);
								//ImGui::NewLine();
								//ImGui::Checkbox(_xor_("Head ESP ").c_str(), &settings::head);

								ImGui::NextColumn();
								ImGui::NewLine();
								ImGui::Checkbox(_xor_("Box ESP ").c_str(), &settings::box);
								//ImGui::NewLine();
								//ImGui::Checkbox(_xor_("Skeleton ESP ").c_str(), &settings::skeleton);
								ImGui::NewLine();
								ImGui::Checkbox(_xor_("Health ESP ").c_str(), &settings::health);

							}
							ImGui::EndChild();

							ImGui::End();
						}
					}
				}
				
				iteration();

				dxcomp::end_scene();
			}

			if (::GetAsyncKeyState(VK_F12) & 0x8000) {
				dxcomp::begin_scene();
				dxcomp::end_scene();
				return true;
			}

			if (::GetAsyncKeyState(VK_INSERT) & 0x8000) {
				menu = !menu;
				Sleep(100);
			}

			if (::GetAsyncKeyState(VK_F8) & 0x8000) {
				panic = !panic;
				Sleep(100);
			}

			if (!::FindWindowA(0, _xor_("VALORANT  ").c_str())) {
				dxcomp::begin_scene();
				dxcomp::end_scene();
				return true;
			}
		}
		return true;
	}
}