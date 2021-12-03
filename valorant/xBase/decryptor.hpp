#pragma once

namespace decryptor {
	extern uint64_t pkey;
	extern uint64_t pstate;

	extern 	__forceinline __int64 decrypt_uworld(const uint32_t key, const uint64_t* state);
	extern __forceinline uint64_t read_uworld(uint64_t main_base);
}