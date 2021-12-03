#include "stdafx.h"

namespace core {
	bool core_get_base_by_id(uint64_t pstruct) {
		_k_get_base_by_id* in = (_k_get_base_by_id*)pstruct;
		_k_get_base_by_id local = { in->pid, in->addr };

		uint64_t base = krnlutils::get_processbase_by_id(local.pid);
		if (!base) { return false; }

		RtlCopyMemory((uint64_t*)local.addr, &base, sizeof(uint64_t));
		return true;
	}

	bool core_ud_memcpy(uint64_t pstruct) {
		NTSTATUS status;
		SIZE_T   return_size = 0;
		PEPROCESS process_src = nullptr;
		PEPROCESS process_dst = nullptr;

		_k_rw_request* in = (_k_rw_request*)pstruct;
		_k_rw_request local = { in->src_pid, in->src_addr, in->dst_pid, in->dst_addr, in->size };

		size_t memsize = 0;
		void* buffer = ExAllocatePoolWithTag(NonPagedPool, in->size, POOLTAG);
		if (!buffer)
			return false;

		// mmcvm equivalent
		{
			// read from source
			if (!NT_SUCCESS(utils::ReadProcessMemory((int)in->src_pid, (void*)in->src_addr, buffer, in->size, &memsize)))
				return false;

			// write to dest
			if (!NT_SUCCESS(utils::WriteProcessMemory((int)in->dst_pid, (void*)in->dst_addr, buffer, in->size, &memsize)))
				return false;
		}

		ExFreePoolWithTag(buffer, POOLTAG);
		return true;
	}

	bool core_virtual_protect(uint64_t pstruct) {
		ImpDef(PsLookupProcessByProcessId);
		ImpDef(KeStackAttachProcess);
		ImpDef(KeUnstackDetachProcess);
		ImpDef(ZwProtectVirtualMemory);
		ImpDef(ObfDereferenceObject);
		ImpSet(PsLookupProcessByProcessId);
		ImpSet(KeStackAttachProcess);
		ImpSet(KeUnstackDetachProcess);
		ImpSet(ZwProtectVirtualMemory);
		ImpSet(ObfDereferenceObject);

		_k_virtual_protect* in = (_k_virtual_protect*)pstruct;
		_k_virtual_protect local = { in->pid, in->protect, in->addr, in->size };

		NTSTATUS status;
		PEPROCESS target = nullptr;
		void* protect_base = (void*)local.addr;

		if (!NT_SUCCESS(ImpCall(PsLookupProcessByProcessId, HANDLE(local.pid), &target))) { return false; }

		KAPC_STATE apc;
		ULONG old_protection = NULL;

		ImpCall(KeStackAttachProcess, target, &apc);
		status = ImpCall(ZwProtectVirtualMemory, ZwCurrentProcess(), &protect_base, &local.size, (ULONG)local.protect, &old_protection);
		ImpCall(KeUnstackDetachProcess, &apc);
		in->protect = old_protection;

		ImpCall(ObfDereferenceObject, target);

		if (NT_SUCCESS(status))
			return true;
		else
			return false;
	}

	bool core_virtual_alloc(uint64_t pstruct) {
		ImpDef(PsLookupProcessByProcessId);
		ImpDef(KeStackAttachProcess);
		ImpDef(KeUnstackDetachProcess);
		ImpDef(ZwAllocateVirtualMemory);
		ImpDef(ObfDereferenceObject);
		ImpDef(MmSecureVirtualMemory);
		ImpSet(PsLookupProcessByProcessId);
		ImpSet(KeStackAttachProcess);
		ImpSet(KeUnstackDetachProcess);
		ImpSet(ZwAllocateVirtualMemory);
		ImpSet(ObfDereferenceObject);
		ImpSet(MmSecureVirtualMemory);


		_k_virtual_alloc* in = (_k_virtual_alloc*)pstruct;
		_k_virtual_alloc local = { in->pid, in->allocation_type, in->protect, in->addr, in->size };

		NTSTATUS status;
		PEPROCESS target = nullptr;
		PVOID alloc_base = NULL;

		if (!NT_SUCCESS(ImpCall(PsLookupProcessByProcessId, HANDLE(local.pid), &target))) { return false; }

		KAPC_STATE apc;
		ImpCall(KeStackAttachProcess, target, &apc);
		status = ImpCall(ZwAllocateVirtualMemory, ZwCurrentProcess(), &alloc_base, 0, &local.size,
			(ULONG)local.allocation_type, (ULONG)local.protect);

		// prevent memory from beeing freed
		{
			ImpCall(MmSecureVirtualMemory, alloc_base, local.size, PAGE_READWRITE);
		}

		ImpCall(KeUnstackDetachProcess, &apc);

		if (alloc_base)
			RtlCopyMemory((uint64_t*)local.addr, &alloc_base, sizeof(uint64_t));

		ImpCall(ObfDereferenceObject, target);

		if (NT_SUCCESS(status))
			return true;
		else
			return false;
	}

	bool core_write_to_readonly(uint64_t pstruct) {
		ImpDef(ExAllocatePoolWithTag);
		ImpDef(PsLookupProcessByProcessId);
		ImpDef(KeStackAttachProcess);
		ImpDef(IoAllocateMdl);
		ImpDef(MmProbeAndLockPages);
		ImpDef(MmMapLockedPagesSpecifyCache);
		ImpDef(ObfDereferenceObject);
		ImpDef(MmUnlockPages);
		ImpDef(IoFreeMdl);
		ImpDef(KeUnstackDetachProcess);
		ImpDef(ExFreePoolWithTag);

		ImpSet(ExAllocatePoolWithTag);
		ImpSet(PsLookupProcessByProcessId);
		ImpSet(KeStackAttachProcess);
		ImpSet(IoAllocateMdl);
		ImpSet(MmProbeAndLockPages);
		ImpSet(MmMapLockedPagesSpecifyCache);
		ImpSet(ObfDereferenceObject);
		ImpSet(MmUnlockPages);
		ImpSet(IoFreeMdl);
		ImpSet(KeUnstackDetachProcess);
		ImpSet(ExFreePoolWithTag);

		PEPROCESS process_src = nullptr;
		PEPROCESS process_dst = nullptr;
		NTSTATUS status;
		SIZE_T   return_size = 0;
		_k_rw_request* in = (_k_rw_request*)pstruct;
		_k_rw_request local = { in->src_pid, in->src_addr, in->dst_pid, in->dst_addr, in->size };

		void* buffer = ImpCall(ExAllocatePoolWithTag, NonPagedPool, local.size, POOLTAG);
		memcpy(buffer, (void*)local.src_addr, local.size);

		if (!NT_SUCCESS(ImpCall(PsLookupProcessByProcessId, HANDLE(in->dst_pid), &process_dst))) { return false; }

		if (!NT_SUCCESS(ImpCall(PsLookupProcessByProcessId, HANDLE(in->src_pid), &process_src))) { return false; }

		KAPC_STATE apc;
		ImpCall(KeStackAttachProcess, process_dst, &apc);

		const auto mdl = ImpCall(IoAllocateMdl, reinterpret_cast<void*>(local.dst_addr), sizeof(local.size),
			false, false, nullptr);
		if (!mdl) {
			ImpCall(ObfDereferenceObject, process_src);
			ImpCall(ObfDereferenceObject, process_dst);
			return false;
		}

		ImpCall(MmProbeAndLockPages, mdl, KernelMode, IoReadAccess);

		const auto targetRemapped = reinterpret_cast<uintptr_t*>(ImpCall(MmMapLockedPagesSpecifyCache, mdl, KernelMode,
			MmNonCached, nullptr, false, HighPagePriority));

		if (!targetRemapped) {
			ImpCall(ObfDereferenceObject, process_src);
			ImpCall(ObfDereferenceObject, process_dst);
			return false;
		}

		memcpy(targetRemapped, buffer, local.size);

		ImpCall(MmUnlockPages, mdl);
		ImpCall(IoFreeMdl, mdl);

		ImpCall(KeUnstackDetachProcess, &apc);

		ImpCall(ExFreePoolWithTag, buffer, POOLTAG);

		ImpCall(ObfDereferenceObject, process_src);
		ImpCall(ObfDereferenceObject, process_dst);

		return true;
	}

	bool core_get_thread(uint64_t pstruct) {
		ImpDef(KeStackAttachProcess);
		ImpDef(KeGetCurrentThread);
		ImpDef(KeUnstackDetachProcess);

		ImpSet(KeStackAttachProcess);
		ImpSet(KeGetCurrentThread);
		ImpSet(KeUnstackDetachProcess);

		_k_gen_thread_ctx* in = (_k_gen_thread_ctx*)pstruct;
		_k_gen_thread_ctx local = { in->window_handle, in->thread_pointer, in->thread_alternative };

		static const auto ValidateHwnd = reinterpret_cast<defs::tag_wnd * (*)(uint64_t)>(
			krnlutils::get_km_module_export(E("win32kbase.sys"), E("ValidateHwnd"))
			);

		if (!ValidateHwnd) { return false; }

		KAPC_STATE apc;
		PEPROCESS pProcess = (PEPROCESS)krnlutils::get_process_by_name(E("dwm.exe"));

		if (!pProcess) { return false; }

		ImpCall(KeStackAttachProcess, pProcess, &apc);

		PETHREAD current = ImpCall(KeGetCurrentThread);
		void* old_win32 = 0;

		PEPROCESS exProcess = 0;
		PETHREAD exThread = 0;

		krnlutils::find_thread(E("dwm.exe"), &exProcess, &exThread);
		utils::set_win32(current, utils::get_win32(exThread), &old_win32);

		byte old_state_index = NULL;
		utils::set_apc_index(current, 0, &old_state_index);

		const auto window_instance = ValidateHwnd(local.window_handle);

		utils::set_win32(current, old_win32, NULL);
		utils::set_apc_index(current, old_state_index, NULL);

		ImpCall(KeUnstackDetachProcess, &apc);

		if (!window_instance || !window_instance->thread_info) { return false; }

		RtlCopyMemory((uint64_t*)&in->thread_pointer, &window_instance->thread_info->owning_thread, sizeof(uint64_t));
		return true;
	}

	bool core_set_thread(uint64_t pstruct) {
		ImpDef(KeStackAttachProcess);
		ImpDef(KeGetCurrentThread);
		ImpDef(KeUnstackDetachProcess);

		ImpSet(KeStackAttachProcess);
		ImpSet(KeGetCurrentThread);
		ImpSet(KeUnstackDetachProcess);

		_k_gen_thread_ctx* in = (_k_gen_thread_ctx*)pstruct;
		_k_gen_thread_ctx local = { in->window_handle, in->thread_pointer, in->thread_alternative };

		static const auto ValidateHwnd = reinterpret_cast<defs::tag_wnd * (*)(uint64_t)>(
			krnlutils::get_km_module_export(E("win32kbase.sys"), E("ValidateHwnd"))
			);

		if (!ValidateHwnd) { return false; }

		KAPC_STATE apc;
		PEPROCESS pProcess = (PEPROCESS)krnlutils::get_process_by_name(E("dwm.exe"));

		if (!pProcess) { return false; }

		ImpCall(KeStackAttachProcess, pProcess, &apc);

		PETHREAD current = ImpCall(KeGetCurrentThread);
		void* old_win32 = 0;

		PEPROCESS exProcess = 0;
		PETHREAD exThread = 0;

		krnlutils::find_thread(E("dwm.exe"), &exProcess, &exThread);
		utils::set_win32(current, utils::get_win32(exThread), &old_win32);

		byte old_state_index = NULL;
		utils::set_apc_index(current, 0, &old_state_index);

		const auto window_instance = ValidateHwnd(local.window_handle);

		utils::set_win32(current, old_win32, NULL);
		utils::set_apc_index(current, old_state_index, NULL);

		ImpCall(KeUnstackDetachProcess, &apc);

		if (!window_instance || !window_instance->thread_info) { return false; }

		window_instance->thread_info->owning_thread = reinterpret_cast<PETHREAD>(local.thread_pointer);
		return true;
	}

	bool core_get_um_module(uint64_t pstruct) {
		ImpDef(PsLookupProcessByProcessId);
		ImpDef(ObfDereferenceObject);

		ImpSet(PsLookupProcessByProcessId);
		ImpSet(ObfDereferenceObject);

		NTSTATUS status;
		PEPROCESS pProcess = nullptr;

		_k_get_um_module* in = (_k_get_um_module*)pstruct;

		status = ImpCall(PsLookupProcessByProcessId, HANDLE(in->pid), &pProcess);

		if (!NT_SUCCESS(status)) {
			return false;
		}

		WCHAR module_name[256] = {};
		wcscpy(module_name, in->moduleName);

		if (!module_name[1]) {
			return false;
		}

		uint32_t moduleSize = (uint32_t)krnlutils::get_um_module_size(pProcess, module_name);
		uintptr_t moduleBase = krnlutils::get_um_module_base(pProcess, module_name);

		if (!moduleBase || !moduleSize) {
			ImpCall(ObfDereferenceObject, pProcess);
			return false;
		}

		RtlCopyMemory((uint64_t*)in->dst_base, &moduleBase, sizeof(uint64_t));
		RtlCopyMemory((uint64_t*)in->dst_size, &moduleSize, sizeof(uint32_t));

		ImpCall(ObfDereferenceObject, pProcess);
		return true;
	}


	typedef NTSTATUS(NTAPI* fnExRemoveHandleTable)(IN defs::PHANDLE_TABLE pTable);
	fnExRemoveHandleTable ExpRemoveHandleTable;

	typedef BOOL(FASTCALL* _CWindowPropGetProp)(__int64 a1, __int64* a2);
	_CWindowPropGetProp getProp;
	bool core_hide_dcomp(uint64_t pstruct) {
		ImpDef(KeStackAttachProcess);
		ImpDef(KeGetCurrentThread);
		ImpDef(KeUnstackDetachProcess);
		ImpDef(PsLookupProcessByProcessId);

		ImpSet(PsLookupProcessByProcessId);
		ImpSet(KeStackAttachProcess);
		ImpSet(KeGetCurrentThread);
		ImpSet(KeUnstackDetachProcess);

		_k_hide_dcomp* in = (_k_hide_dcomp*)pstruct;
		_k_hide_dcomp local = { in->pid_hide_handle, in->window_handle};

		ExpRemoveHandleTable = (fnExRemoveHandleTable)utils::scan_pattern((uint8_t*)kbase, 
			0xC00000, 
			"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x65\x48\x8B\x34\x25\x88\x01\x00\x00\x48\x83\xCB\xFF\x48\x8B\xF9",
			E("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));

		if (!ExpRemoveHandleTable) { return false; }

		PEPROCESS current{};
		if(!NT_SUCCESS(ImpCall(PsLookupProcessByProcessId, (HANDLE)local.pid_hide_handle, &current))) { return false; }
		if(!current){ return false; }

		// hide processes handle
		{
			defs::PHANDLE_TABLE pHandleTable = *(defs::PHANDLE_TABLE*)((PUCHAR)current + 0x570);
			pHandleTable->FreeLists->HandleCount = 0;
			ExpRemoveHandleTable(pHandleTable);
		}

		// hide setprop flags
		{
			static const auto ValidateHwnd = reinterpret_cast<defs::tag_wnd * (*)(uint64_t)>(
				krnlutils::get_km_module_export(E("win32kbase.sys"), E("ValidateHwnd"))
				);

			if (!ValidateHwnd) { return false; }

			KAPC_STATE apc;
			PEPROCESS pProcess = (PEPROCESS)krnlutils::get_process_by_name(E("dwm.exe"));

			if (!pProcess) { return false; }

			ImpCall(KeStackAttachProcess, pProcess, &apc);

			PETHREAD current = ImpCall(KeGetCurrentThread);
			void* old_win32 = 0;

			PEPROCESS exProcess = 0;
			PETHREAD exThread = 0;
			size_t win32kfullsize{};
			uintptr_t win32kfullbase{};
			win32kfullbase = krnlutils::get_km_module(E("win32kfull.sys"), win32kfullsize);
			if (!win32kfullbase) { ImpCall(KeUnstackDetachProcess, &apc); return false; }

			getProp = (_CWindowPropGetProp)utils::scan_pattern((uint8_t*)win32kfullbase,
				win32kfullsize,
				"\x40\x53\x48\x83\xEC\x20\x48\x8B\x05\xCC\xCC\xCC\xCC\x48\x8B\xDA\x41\xB8\x01\x00\x00\x00",
				E("xxxxxxxxx????xxxxxxxxx"));
			if (!getProp) { ImpCall(KeUnstackDetachProcess, &apc); return false; }

			krnlutils::find_thread(E("dwm.exe"), &exProcess, &exThread);
			utils::set_win32(current, utils::get_win32(exThread), &old_win32);

			byte old_state_index = NULL;
			utils::set_apc_index(current, 0, &old_state_index);

			uintptr_t tagWND = (uintptr_t)ValidateHwnd(local.window_handle);
			uintptr_t target_info = 0;
			getProp(tagWND, (__int64*)&target_info);

			UINT_PTR flag1 = *(UINT_PTR*)(target_info + 0x10);
			UINT_PTR flag2 = *(UINT_PTR*)(target_info + 0x18);

			*(UINT_PTR*)(target_info + 0x10) = 0;
			*(UINT_PTR*)(target_info + 0x18) = 0;

			utils::set_win32(current, old_win32, NULL);
			utils::set_apc_index(current, old_state_index, NULL);

			ImpCall(KeUnstackDetachProcess, &apc);
		}

		return true;
	}

	bool core_dispatcher(uint64_t pstruct, uint32_t id) {
		switch (id) {
		case DRIVER_GET_BASE_BY_ID:
			return core_get_base_by_id(pstruct);
			break;

		case DRIVER_MEM_CPY:
			return core_ud_memcpy(pstruct);
			break;

		case DRIVER_PROTECT:
			return core_virtual_protect(pstruct);
			break;

		case DRIVER_ALLOC:
			return core_virtual_alloc(pstruct);
			break;

		case DRIVER_CPY_TO_READONLY:
			return core_write_to_readonly(pstruct);
			break;

		case DRIVER_GET_THREAD:
			return core_get_thread(pstruct);
			break;

		case DRIVER_SET_THREAD:
			return core_set_thread(pstruct);
			break;

		case DRIVER_GET_UM_MODULE:
			return core_get_um_module(pstruct);
			break;

		case DRIVER_INIT:
			utils::sys_delay(1500);
			break;

		case DRIVER_HIDE_DCOMP:
			return core_hide_dcomp(pstruct);
			break;
		}
		return true;
	}
}