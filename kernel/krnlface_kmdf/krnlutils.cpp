#include "stdafx.h"

using namespace defs;

namespace krnlutils {
	uintptr_t get_process_by_name(const char* process_name) {
		ImpDef(_stricmp);
		ImpSet(_stricmp);

		if (!global::eprocess::o_activeprocesslinks || !global::eprocess::o_imagefilename)
			return 0;

		uintptr_t list_head = *(uintptr_t*)((uintptr_t)PsInitialSystemProcess + global::eprocess::o_activeprocesslinks);
		uintptr_t list_current = list_head;

		do
		{
			uintptr_t list_entry = list_current - global::eprocess::o_activeprocesslinks;
			if (!ImpCall(_stricmp, process_name, (char*)(list_entry + global::eprocess::o_imagefilename)))
			{
				return list_entry;
			}
			list_current = *(uintptr_t*)list_current;
		} while (list_current != list_head);
		return 0;
	}

	uintptr_t get_processbase(PEPROCESS pprocess) {
		ImpDef(PsGetProcessSectionBaseAddress);
		ImpSet(PsGetProcessSectionBaseAddress);
		return reinterpret_cast<uintptr_t>(ImpCall(PsGetProcessSectionBaseAddress, pprocess));
	}

	uintptr_t get_processbase_by_id(uint32_t process_id) {
		ImpDef(PsLookupProcessByProcessId);
		ImpDef(PsGetProcessSectionBaseAddress);
		ImpDef(ObfDereferenceObject);

		ImpSet(PsLookupProcessByProcessId);
		ImpSet(PsGetProcessSectionBaseAddress);
		ImpSet(ObfDereferenceObject);

		PEPROCESS pProcess;
		if (NT_SUCCESS(ImpCall(PsLookupProcessByProcessId,(HANDLE)process_id, &pProcess))) {
			uintptr_t base = (uintptr_t)ImpCall(PsGetProcessSectionBaseAddress, pProcess);
			ImpCall(ObfDereferenceObject, pProcess);
			return base;
		}
		return 0;
	}

	uintptr_t get_km_module(const char* module_name, size_t& module_size) {
		ImpDef(ZwQuerySystemInformation);
		ImpDef(ExAllocatePoolWithTag);
		ImpDef(ExFreePoolWithTag);

		ImpSet(ZwQuerySystemInformation);
		ImpSet(ExAllocatePoolWithTag);
		ImpSet(ExFreePoolWithTag);

		ULONG bytes = 0;
		NTSTATUS status = ImpCall(ZwQuerySystemInformation, defs::SystemModuleInformation, 0, bytes, &bytes);

		if (!bytes)
			return 0;

		PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ImpCall(ExAllocatePoolWithTag, NonPagedPool, bytes, POOLTAG);
		status = ImpCall(ZwQuerySystemInformation, defs::SystemModuleInformation, modules, bytes, &bytes);

		if (!NT_SUCCESS(status)) {
			ImpCall(ExFreePoolWithTag, modules, POOLTAG);
			return 0;
		}

		PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
		uintptr_t module_base = 0;
		HANDLE pid = 0;

		for (ULONG i = 0; i < modules->NumberOfModules; i++)
		{
			if (strcmp((char*)(module[i].FullPathName + module[i].OffsetToFileName), module_name) == 0)
			{
				module_base = uintptr_t(module[i].ImageBase);
				module_size = module[i].ImageSize;
				break;
			}
		}

		if (modules)
			ImpCall(ExFreePoolWithTag, modules, POOLTAG);

		if (module_base <= 0)
			return 0;

		return module_base;
	}

	uintptr_t get_km_module_export(const char* module_name, const char* routine_name) {
		ImpDef(RtlFindExportedRoutineByName);

		ImpSet(RtlFindExportedRoutineByName);

		size_t size;
		uintptr_t lpModule = get_km_module(module_name, size);

		if (!lpModule)
			return NULL;

		return (uintptr_t)ImpCall(RtlFindExportedRoutineByName, (PVOID)lpModule, routine_name);
	}

	uintptr_t get_um_module_base(PEPROCESS pprocess, LPCWSTR module_name) {
		ImpDef(KeAttachProcess);
		ImpDef(PsGetProcessPeb);
		ImpDef(KeDetachProcess);
		ImpDef(RtlInitUnicodeString);
		ImpDef(RtlCompareUnicodeString);

		ImpSet(KeAttachProcess);
		ImpSet(PsGetProcessPeb);
		ImpSet(KeDetachProcess);
		ImpSet(RtlInitUnicodeString);
		ImpSet(RtlCompareUnicodeString);

		uintptr_t base;

		if (!pprocess) {
			return 0;
		}

		ImpCall(KeAttachProcess, (PKPROCESS)pprocess);

		defs::PPEB peb = (defs::PPEB)ImpCall(PsGetProcessPeb, pprocess);
		if (!peb) {
			ImpCall(KeDetachProcess);
			return 0;
		}

		if (!peb->Ldr || !peb->Ldr->Initialized) {
			ImpCall(KeDetachProcess);
			return 0;
		}

		if (!module_name) {
			return 0;
		}

		UNICODE_STRING module_name_unicode;
		ImpCall(RtlInitUnicodeString, &module_name_unicode, module_name);

		for (PLIST_ENTRY list = peb->Ldr->ModuleListLoadOrder.Flink;
			list != &peb->Ldr->ModuleListLoadOrder;
			list = list->Flink) {
			PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(list, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
			if (ImpCall(RtlCompareUnicodeString, &entry->BaseDllName, &module_name_unicode, TRUE) == 0) {
				base = (uintptr_t)entry->DllBase;
				ImpCall(KeDetachProcess);
				return base;
			}
		}

		ImpCall(KeDetachProcess);
		return 0;
	}

	uintptr_t get_um_module_size(PEPROCESS pprocess, LPCWSTR module_name) {
		ImpDef(KeAttachProcess);
		ImpDef(PsGetProcessPeb);
		ImpDef(KeDetachProcess);
		ImpDef(RtlInitUnicodeString);
		ImpDef(RtlCompareUnicodeString);

		ImpSet(KeAttachProcess);
		ImpSet(PsGetProcessPeb);
		ImpSet(KeDetachProcess);
		ImpSet(RtlInitUnicodeString);
		ImpSet(RtlCompareUnicodeString);

		uintptr_t size;

		if (!pprocess) {
			return 0;
		}

		ImpCall(KeAttachProcess, (PKPROCESS)pprocess);

		defs::PPEB peb = (defs::PPEB)ImpCall(PsGetProcessPeb, pprocess);
		if (!peb) {
			ImpCall(KeDetachProcess);
			return 0;
		}

		if (!peb->Ldr || !peb->Ldr->Initialized) {
			ImpCall(KeDetachProcess);
			return 0;
		}

		if (!module_name) {
			return 0;
		}

		UNICODE_STRING module_name_unicode;
		ImpCall(RtlInitUnicodeString, &module_name_unicode, module_name);

		for (PLIST_ENTRY list = peb->Ldr->ModuleListLoadOrder.Flink;
			list != &peb->Ldr->ModuleListLoadOrder;
			list = list->Flink) {
			PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(list, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
			if (ImpCall(RtlCompareUnicodeString, &entry->BaseDllName, &module_name_unicode, TRUE) == 0) {
				size = (uintptr_t)entry->SizeOfImage;
				ImpCall(KeDetachProcess);
				return size;
			}
		}

		ImpCall(KeDetachProcess);
		return 0;
	}

	int find_thread(char* ProcessName, PEPROCESS* Process, PETHREAD* Thread) {
		ImpDef(IoGetCurrentProcess);
		ImpDef(PsIsSystemThread);
		ImpDef(_stricmp);

		ImpSet(IoGetCurrentProcess);
		ImpSet(PsIsSystemThread);
		ImpSet(_stricmp);

		if (!global::eprocess::o_activeprocesslinks || !global::eprocess::o_imagefilename)
			return 0;

		PLIST_ENTRY CurrentProcess, StartProcess;
		PLIST_ENTRY   ThreadList, StartThread;
		BOOLEAN FoundUserModeThread = FALSE;
		DWORD TcbAddress;

		*Process = nullptr;
		*Thread = nullptr;
  
		StartProcess = CurrentProcess = (PLIST_ENTRY)((BYTE*)ImpCall(IoGetCurrentProcess) + global::eprocess::o_activeprocesslinks);
		
		uintptr_t list_head = *(uintptr_t*)((uintptr_t)PsInitialSystemProcess + global::eprocess::o_activeprocesslinks);
		uintptr_t list_current = list_head;

		do
		{
			uintptr_t list_entry = list_current - global::eprocess::o_activeprocesslinks;
			if (!ImpCall(_stricmp, ProcessName, (char*)(list_entry + global::eprocess::o_imagefilename)))
			{
				*Process = (PEPROCESS)((PBYTE)list_entry - global::eprocess::o_activeprocesslinks);
				CurrentProcess = (PLIST_ENTRY)list_entry;
				break;
			}
			list_current = *(uintptr_t*)list_current;
		} while (list_current != list_head);

		if (*Process == NULL)
			return FALSE;
 
		StartThread = ThreadList = ((LIST_ENTRY*)((BYTE*)CurrentProcess + global::ethread::o_threadlisthead))->Flink;

		do
		{
			*Thread = (PETHREAD)((PBYTE)ThreadList - global::ethread::o_threadlistentry);
			if (!ImpCall(PsIsSystemThread, *Thread))
			{
				return TRUE;
			}
			ThreadList = ThreadList->Flink;
		} while (ThreadList != StartThread);

		return FALSE;
	}

	bool memcpy_to_readonly(PVOID dest, PVOID src, ULONG length) {
		ImpDef(IoAllocateMdl);
		ImpDef(MmMapLockedPagesSpecifyCache);	
		ImpDef(MmProbeAndLockPages);
		ImpDef(MmUnlockPages);
		ImpDef(IoFreeMdl);
		ImpDef(MmUnmapLockedPages);

		ImpSet(IoAllocateMdl);
		ImpSet(MmMapLockedPagesSpecifyCache);
		ImpSet(MmProbeAndLockPages);
		ImpSet(MmUnlockPages);
		ImpSet(IoFreeMdl);
		ImpSet(MmUnmapLockedPages); 

		PMDL mdl = ImpCall(IoAllocateMdl, dest, length, FALSE, FALSE, NULL);
		if (!mdl) {
			return FALSE;
		}

		ImpCall(MmProbeAndLockPages, mdl, KernelMode, IoModifyAccess);

		PVOID mapped = ImpCall(MmMapLockedPagesSpecifyCache, mdl, KernelMode, MmNonCached, NULL, 0, HighPagePriority);
		if (!mapped) {
			ImpCall(MmUnlockPages, mdl);
			ImpCall(IoFreeMdl, mdl);
			return FALSE;
		}

		memcpy(mapped, src, length);

		ImpCall(MmUnmapLockedPages, mapped, mdl);
		ImpCall(MmUnlockPages, mdl);
		ImpCall(IoFreeMdl, mdl);
		return TRUE;
	}

	ULONG64 get_krnl_base_no_imports()
	{
		ULONG64 CurPos = __readmsr(0xC0000082) & ~0xfff;
		while (true)
		{
			if (*(USHORT*)CurPos == 0x5A4D)
			{
				for (ULONG64 i = CurPos; i < CurPos + 0x400; i += 8)
				{
					if (*(ULONG64*)i == 0x45444F434C4F4F50)
					{
						return CurPos;
					}
				}
			}
			CurPos -= 0x1000;
		}
		return 0;
	}

#define NT_HEADER(ModBase) (PIMAGE_NT_HEADERS)((ULONG64)(ModBase) + ((PIMAGE_DOS_HEADER)(ModBase))->e_lfanew)
	void* km_get_proc_address(void* mod_base, const char* name) {
		PIMAGE_NT_HEADERS NT_Head = NT_HEADER(mod_base);
		PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)((ULONG64)mod_base + NT_Head->OptionalHeader.DataDirectory[0].VirtualAddress);

		for (ULONG i = 0; i < ExportDir->NumberOfNames; i++)
		{
			USHORT Ordinal = ((USHORT*)((ULONG64)mod_base + ExportDir->AddressOfNameOrdinals))[i];
			const char* ExpName = (const char*)mod_base + ((ULONG*)((ULONG64)mod_base + ExportDir->AddressOfNames))[i];

			if (utils::StrICmp(name, ExpName, true))
				return (PVOID)((ULONG64)mod_base + ((ULONG*)((ULONG64)mod_base + ExportDir->AddressOfFunctions))[Ordinal]);
		}
		return nullptr;
	}
}