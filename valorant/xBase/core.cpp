#include "stdafx.hpp"

namespace core {
	nt_user_init ntusrinit = nullptr;

	bool core_init() {
		ntusrinit = (nt_user_init)::GetProcAddress(::LoadLibraryA(_xor_("win32u.dll").c_str()), _xor_("NtGdiFlush").c_str());
		if (!ntusrinit) {
			::printf(_xor_("[-] API not found!\n").c_str());
			return false;
		}
		else
		{
			std::clock_t start;
			start = std::clock();
			INT64 state = ntusrinit(0xDEADBEEF + DRIVER_INIT, 0xFFFFFFFFFF);
			if ((std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) > 100) {
				return true;
			}
			//if (ntusrinit(0xDEADBEEF + DRIVER_INIT, 0xFFFFFFFFFF) == 0x69)
			//	return true;
		}
		return false;
	}

	uint64_t get_process_base_by_id(uint32_t pid) {
		uint64_t base = 0;
		_k_get_base_by_id out = { pid, (uint64_t)&base };

		uint64_t status = ntusrinit(0xDEADBEEF + DRIVER_GET_BASE_BY_ID, reinterpret_cast<uintptr_t>(&out));
		return base;
	}

	uint64_t virtual_alloc(uint32_t process_id, uint32_t allocation_type, uint32_t protect, size_t size) {
		uint64_t base = 0;
		_k_virtual_alloc out = { process_id, allocation_type, protect, (uint64_t)&base, size };

		uint64_t status = ntusrinit(0xDEADBEEF + DRIVER_ALLOC, reinterpret_cast<uintptr_t>(&out));
		return base;
	}

	bool get_um_module(uint32_t pid, const char* module_name, uint64_t& base, uint32_t& size) {
		uint64_t mod_base = NULL;
		uint32_t mod_size = NULL;
		_k_get_um_module out = {};

		wchar_t* wc = utils::getwc(module_name);

		memset(out.moduleName, 0, sizeof(WCHAR) * 256);
		wcscpy(out.moduleName, wc);

		out.dst_base = (uint64_t)&mod_base;
		out.dst_size = (uint64_t)&mod_size;
		out.pid = pid;

		uint64_t status = ntusrinit(0xDEADBEEF + DRIVER_GET_UM_MODULE, reinterpret_cast<uintptr_t>(&out));

		base = mod_base;
		size = mod_size;

		//return status == 0x69 ? true : false;
		return true;
	}

	bool mem_cpy(uint32_t src_pid, uint64_t src_addr, uint32_t dst_pid, uint64_t dst_addr, size_t size) {
		_k_rw_request out = { src_pid, src_addr, dst_pid, dst_addr, size };
		uint64_t status = ntusrinit(0xDEADBEEF + DRIVER_MEM_CPY, reinterpret_cast<uintptr_t>(&out));
		return true;
	}

	bool mem_cpy_readonly(uint32_t src_pid, uint64_t src_addr, uint32_t dst_pid, uint64_t dst_addr, size_t size) {
		_k_rw_request out = { src_pid, src_addr, dst_pid, dst_addr, size };
		uint64_t status = ntusrinit(0xDEADBEEF + DRIVER_CPY_TO_READONLY, reinterpret_cast<uintptr_t>(&out));

		//return status == 0x69 ? true : false;
		return true;
	}

	bool virtual_protect(uint32_t process_id, uintptr_t address, uint32_t protect, size_t size) {
		_k_virtual_protect out = { process_id, protect, address, size };

		uint64_t status = ntusrinit(0xDEADBEEF + DRIVER_PROTECT, reinterpret_cast<uintptr_t>(&out));

		//return status == 0x69 ? true : false;
		return true;
	}

	bool get_thread(HWND window_handle, uint64_t* thread_context) {
		_k_gen_thread_ctx out{};
		out.window_handle = reinterpret_cast<uint64_t>(window_handle);
		out.thread_pointer = 10;
		out.thread_alternative = 0;

		uint64_t status = ntusrinit(0xDEADBEEF + DRIVER_GET_THREAD, reinterpret_cast<uintptr_t>(&out));

		*thread_context = out.thread_pointer;
		//return status == 0x69 ? true : false;
		return true;
	}

	bool set_thread(HWND window_handle, uint64_t thread_context) {
		_k_gen_thread_ctx out{};

		out.window_handle = reinterpret_cast<uint64_t>(window_handle);
		out.thread_pointer = thread_context;
		out.thread_alternative = thread_context;

		uint64_t status = ntusrinit(0xDEADBEEF + DRIVER_SET_THREAD, reinterpret_cast<uintptr_t>(&out));
		//return status == 0x69 ? true : false;
		return true;
	}

	bool hide_dcomp(uint32_t pid, HWND window_handle) {
		_k_hide_dcomp out{ pid, reinterpret_cast<uint64_t>(window_handle) };

		uint64_t status = ntusrinit(0xDEADBEEF + DRIVER_HIDE_DCOMP, reinterpret_cast<uintptr_t>(&out));
		//return status == 0x69 ? true : false;
		return true;
	}
}