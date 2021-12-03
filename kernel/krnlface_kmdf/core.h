#pragma once

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
#endif

	bool core_dispatcher(uint64_t pstruct, uint32_t id);
}