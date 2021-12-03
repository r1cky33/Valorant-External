#pragma once

namespace magic {
	extern byte magic[];

#ifndef MAGIC_STRUCTS
#define MAGIC_STRUCTS
#pragma pack(push, 1)
	struct ArrayHeader
	{
		uint64_t Ptr;
		uint32_t Size;
	};

	struct Camera {
		Vector3 position;
		Vector3 rotation;
		float fov;
	};

	struct ControlRotation {
		Vector3 ctrl_rotation;
		Vector3 write_ctrl_rotation;
		int32_t writeflag;
	};

	struct HijackState
	{
		ArrayHeader actors;			// 0xC
		uint64_t playerstate;		// 0x14
		Camera camera;				// 0x30
		ControlRotation rotation;	
		uintptr_t localpawn;
	};
#pragma pack(pop)
#endif // !MAGIC_STRUCTS

	extern bool magic_scan(uintptr_t& worldcrypt_key, uintptr_t& worldcrypt_state);
	extern bool write_shell(uint64_t decrypted_world, uint64_t base, uintptr_t& pentitycache, uintptr_t& plocalproxy);
	extern HijackState read_results();
}