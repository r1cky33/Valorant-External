#pragma once

typedef INT64(*nt_user_init)(uintptr_t, uintptr_t);

namespace core {
#ifndef DRIVER_GET_BASE_BY_ID
#define DRIVER_GET_BASE_BY_ID		1
#define DRIVER_MEM_CPY				2
#define DRIVER_PROTECT				3
#define DRIVER_ALLOC				4
#define DRIVER_CPY_TO_READONLY		5
#define DRIVER_GET_THREAD			6
#define DRIVER_SET_THREAD			7
#define DRIVER_GET_UM_MODULE		8
#define DRIVER_INIT					10
#define DRIVER_HIDE_DCOMP			11
#endif

#ifndef SHARED_DEFS
#define SHARED_DEFS
	struct _k_rw_request {
		uint32_t	src_pid;
		uint64_t	src_addr;
		uint32_t	dst_pid;
		uint64_t	dst_addr;
		size_t		size;
	};

	struct _k_virtual_alloc {
		uint32_t pid;
		uint32_t allocation_type;
		uint32_t protect;
		uint64_t addr;
		size_t size;
	};

	struct _k_virtual_protect {
		uint32_t pid;
		uint32_t protect;
		uint64_t addr;
		size_t size;
	};

	struct _k_get_base_by_id {
		uint32_t pid;
		uint64_t addr;
	};

	struct _k_get_um_module {
		uint32_t pid;
		WCHAR	 moduleName[256];
		uint64_t	dst_base;
		uint64_t	dst_size;
	};

	struct _k_gen_thread_ctx {
		uint64_t window_handle;
		uint64_t thread_pointer;
		uint64_t thread_alternative;
	};

	struct _k_hide_dcomp {
		uint32_t pid_hide_handle;
		uint64_t window_handle;
	};
#endif // !SHARED_DEFS

	bool core_init();

	uint64_t get_process_base_by_id(uint32_t pid);
	uint64_t virtual_alloc(uint32_t process_id, uint32_t allocation_type, uint32_t protect, size_t size);

	bool get_um_module(uint32_t pid, const char* module_name, uint64_t& base, uint32_t& size);
	bool mem_cpy(uint32_t src_pid, uint64_t src_addr, uint32_t dst_pid, uint64_t dst_addr, size_t size);
	bool mem_cpy_readonly(uint32_t src_pid, uint64_t src_addr, uint32_t dst_pid, uint64_t dst_addr, size_t size);
	bool virtual_protect(uint32_t process_id, uintptr_t address, uint32_t protect, size_t size);

	bool get_thread(HWND window_handle, uint64_t* thread_context);
	bool set_thread(HWND window_handle, uint64_t thread_context);

	bool hide_dcomp(uint32_t pid, HWND window_handle);

	template <typename T>
	T read(const uint32_t process_id, const uintptr_t src, size_t size = sizeof(T))
	{
		T buffer;
		mem_cpy(process_id, src, GetCurrentProcessId(), (uintptr_t)&buffer, size);
		return buffer;
	}

	template <typename T>
	void write(const uint32_t process_id, const uintptr_t src, const uintptr_t dst, size_t size)
	{
		mem_cpy(GetCurrentProcessId(), src, process_id, dst, size);
	}

	template <typename T>	//critical
	bool write_to_readonly(const uint32_t process_id, const uintptr_t src, const uintptr_t dst, size_t size)
	{
		return mem_cpy_readonly(GetCurrentProcessId(), src, process_id, dst, size);
	}
}