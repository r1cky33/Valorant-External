#include "stdafx.hpp"
#include "idadefs.hpp"

namespace decryptor {
	uint64_t pkey = 0;
	uint64_t pstate = 0;

	__forceinline __int64 decrypt_uworld(const uint32_t key, const uint64_t* state)
	{
		unsigned __int64 v19; // rcx
		unsigned __int64 v20; // rdi
		__int64 v21; // r8
		unsigned __int64 v22; // r10
		unsigned __int64 v23; // r11
		unsigned __int64 v24; // r8
		unsigned __int64 v25; // r10
		unsigned __int64 v26; // rcx
		unsigned __int64 v27; // rdx
		v19 = 2685821657736338717i64
			* ((unsigned int)key ^ (unsigned int)(key << 25) ^ (((unsigned int)key ^ ((unsigned __int64)(unsigned int)key >> 15)) >> 12))
			% 7;
		v20 = state[v19];
		v21 = (2685821657736338717i64
			* ((unsigned int)key ^ (unsigned int)(key << 25) ^ (((unsigned int)key ^ ((unsigned __int64)(unsigned int)key >> 15)) >> 12))) >> 32;
		switch ((unsigned int)v19 % 7)
		{
		case 0u:
			v22 = v20 - (unsigned int)(v21 - 1);
			goto LABEL_25;
		case 1u:
			v20 = __ROL8__(v20 - (unsigned int)(v21 + 2 * v19), (unsigned __int8)(((int)v21 + (int)v19) % 0x3Fu) + 1);
			break;
		case 2u:
			v20 = ~(v20 - (unsigned int)(v21 + 2 * v19));
			break;
		case 3u:
			v26 = 2 * ((2 * v20) ^ ((2 * v20) ^ (v20 >> 1)) & 0x5555555555555555i64);
			v20 = v26 ^ (v26 ^ (((2 * v20) ^ ((2 * v20) ^ (v20 >> 1)) & 0x5555555555555555i64) >> 1)) & 0x5555555555555555i64;
			break;
		case 4u:
			v27 = __ROR8__(v20, (unsigned __int8)(((int)v21 + 2 * (int)v19) % 0x3Fu) + 1);
			v20 = (2 * v27) ^ ((2 * v27) ^ (v27 >> 1)) & 0x5555555555555555i64;
			break;
		case 5u:
			v22 = __ROR8__(v20, (unsigned __int8)(((int)v21 + 2 * (int)v19) % 0x3Fu) + 1);
		LABEL_25:
			v23 = (2 * v22) ^ ((2 * v22) ^ (v22 >> 1)) & 0x5555555555555555i64;
			v24 = (4 * v23) ^ ((4 * v23) ^ (v23 >> 2)) & 0x3333333333333333i64;
			v25 = (16 * v24) ^ ((16 * v24) ^ (v24 >> 4)) & 0xF0F0F0F0F0F0F0Fi64;
			v20 = __ROL8__((v25 << 8) ^ ((v25 << 8) ^ (v25 >> 8)) & 0xFF00FF00FF00FFi64, 32);
			break;
		case 6u:
			v20 = ~v20 - (unsigned int)(v21 + v19);
			break;
		default:
			break;
		}
		return v20 ^ (unsigned int)key;
	}

	__forceinline uint64_t read_uworld(uint64_t main_base)
	{
		uint64_t key = core::read<uint64_t>(globals::target_proc::target_process_id, pkey);
		if (!key) { return 0; }

#pragma pack(push, 1)
		struct State
		{
			uint64_t Keys[7];
		};
#pragma pack(pop)
		State state = { 0 };
		state = core::read<State>(globals::target_proc::target_process_id, pstate);

		uintptr_t decrypt = decrypt_uworld(key, (const uint64_t*)&state);
		return decrypt;
	}
}
