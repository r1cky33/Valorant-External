#pragma once

#define STR_XOR_KEY 0x6f

#ifndef _PROTECTED_FUNCTION
#define _PROTECTED_FUNCTION

namespace memprotect {
	struct protected_function {
		void* address;
		size_t size;
		BYTE lastXor;
		bool crypted;
	};
#endif

	extern uint32_t func_count;
	extern protected_function functions[50];

	void add_func(protected_function func);

	void unsafe_unprotect(uint32_t index);
	void unsafe_protect(uint32_t index);

	uint32_t get_func_index(void* func_address);

	void unprotect(void* func_address);
	void protect(void* func_address);

	void xormem (BYTE* data, size_t size, BYTE XOR_KEY = STR_XOR_KEY);

	void protected_sleep(uint32_t ms);
}