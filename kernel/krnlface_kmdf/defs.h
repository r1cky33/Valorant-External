#pragma once

namespace defs {
	typedef enum _SYS_INFO_CLASS {
		SystemBasicInformation,
		SystemProcessorInformation,
		SystemPerformanceInformation,
		SystemTimeOfDayInformation,
		SystemPathInformation,
		SystemProcessInformation,
		SystemCallCountInformation,
		SystemDeviceInformation,
		SystemProcessorPerformanceInformation,
		SystemFlagsInformation,
		SystemCallTimeInformation,
		SystemModuleInformation,
		SystemLocksInformation,
		SystemStackTraceInformation,
		SystemPagedPoolInformation,
		SystemNonPagedPoolInformation,
		SystemHandleInformation,
		SystemObjectInformation,
		SystemPageFileInformation,
		SystemVdmInstemulInformation,
		SystemVdmBopInformation,
		SystemFileCacheInformation,
		SystemPoolTagInformation,
		SystemInterruptInformation,
		SystemDpcBehaviorInformation,
		SystemFullMemoryInformation,
		SystemLoadGdiDriverInformation,
		SystemUnloadGdiDriverInformation,
		SystemTimeAdjustmentInformation,
		SystemSummaryMemoryInformation,
		SystemNextEventIdInformation,
		SystemEventIdsInformation,
		SystemCrashDumpInformation,
		SystemExceptionInformation,
		SystemCrashDumpStateInformation,
		SystemKernelDebuggerInformation,
		SystemContextSwitchInformation,
		SystemRegistryQuotaInformation,
		SystemExtendServiceTableInformation,
		SystemPrioritySeperation,
		SystemPlugPlayBusInformation,
		SystemDockInformation
	} SYS_INFO_CLASS, * PSYS_INFO_CLASS;

	typedef struct _LDR_DATA_TABLE_ENTRY
	{
		LIST_ENTRY InLoadOrderLinks;
		LIST_ENTRY InMemoryOrderLinks;
		LIST_ENTRY InInitializationOrderLinks;
		PVOID DllBase;
		PVOID EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
		ULONG Flags;
		WORD LoadCount;
		WORD TlsIndex;
		union
		{
			LIST_ENTRY HashLinks;
			struct
			{
				PVOID SectionPointer;
				ULONG CheckSum;
			};
		};
		union
		{
			ULONG TimeDateStamp;
			PVOID LoadedImports;
		};
		PVOID EntryPointActivationContext;
		PVOID PatchInformation;
		LIST_ENTRY ForwarderLinks;
		LIST_ENTRY ServiceTagLinks;
		LIST_ENTRY StaticLinks;
	} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;


	//FOR FINDING SYSTEM MODULES
	typedef struct _RTL_PROCESS_MODULE_INFORMATION
	{
		HANDLE Section;
		PVOID MappedBase;
		PVOID ImageBase;
		ULONG ImageSize;
		ULONG Flags;
		USHORT LoadOrderIndex;
		USHORT InitOrderIndex;
		USHORT LoadCount;
		USHORT OffsetToFileName;
		UCHAR  FullPathName[256];
	} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

	typedef struct _RTL_PROCESS_MODULES
	{
		ULONG NumberOfModules;
		RTL_PROCESS_MODULE_INFORMATION Modules[1];
	} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

	//ITERATING PEB_LDR FOR FINDING UM MODULES
	typedef struct _PEB_LDR_DATA {
		ULONG Length;
		BOOLEAN Initialized;
		PVOID SsHandle;
		LIST_ENTRY ModuleListLoadOrder;
		LIST_ENTRY ModuleListMemoryOrder;
		LIST_ENTRY ModuleListInitOrder;
	} PEB_LDR_DATA, * PPEB_LDR_DATA;

	typedef struct _RTL_USER_PROCESS_PARAMETERS {
		BYTE Reserved1[16];
		PVOID Reserved2[10];
		UNICODE_STRING ImagePathName;
		UNICODE_STRING CommandLine;
	} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

	typedef void(__stdcall* PPS_POST_PROCESS_INIT_ROUTINE)(void);

	typedef struct _PEB {
		BYTE Reserved1[2];
		BYTE BeingDebugged;
		BYTE Reserved2[1];
		PVOID Reserved3[2];
		PPEB_LDR_DATA Ldr;
		PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
		PVOID Reserved4[3];
		PVOID AtlThunkSListPtr;
		PVOID Reserved5;
		ULONG Reserved6;
		PVOID Reserved7;
		ULONG Reserved8;
		ULONG AtlThunkSListPtr32;
		PVOID Reserved9[45];
		BYTE Reserved10[96];
		PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
		BYTE Reserved11[128];
		PVOID Reserved12[1];
		ULONG SessionId;
	} PEB, * PPEB;

	// Win32k structures
	typedef struct _wnd_user_info {
		HANDLE window;
		HANDLE region;
		char unk1[0x8];
		DWORD exstyle;
		DWORD style;
		void* instance_handle;
		char unk2[0x50];
		void* wnd_procedure;
	} wnd_user_info, * pwnd_user_info;

	struct tag_thread_info
	{
		PETHREAD owning_thread;
	};

	struct tag_wnd
	{
		HANDLE window;
		void* win32_thread;
		tag_thread_info* thread_info;
		char unk1[0x8];
		tag_wnd* self;
		wnd_user_info* user_info;
		HANDLE region;
		void* region_info;
		tag_wnd* parent;
		tag_wnd* next;
		void* unk2;
		tag_wnd* child;
		tag_wnd* previous;
		void* unk3;
		void* win32;
		void* global_info_link;
		char unk4[0x48];
		DWORD user_procedures_link;
		char unk5[0x1c];
		DWORD procedure_flag;
		char unk6[0x3C];
		void* procedure_table;
	};

#ifdef _WIN64
#define HANDLE_LOW_BITS (PAGE_SHIFT - 4)
#define HANDLE_HIGH_BITS (PAGE_SHIFT - 3)
#else
#define HANDLE_LOW_BITS (PAGE_SHIFT - 3)
#define HANDLE_HIGH_BITS (PAGE_SHIFT - 2)
#endif
#define HANDLE_TAG_BITS (2)
#define HANDLE_INDEX_BITS (HANDLE_LOW_BITS + 2*HANDLE_HIGH_BITS)
#define KERNEL_FLAG_BITS (sizeof(PVOID)*8 - HANDLE_INDEX_BITS - HANDLE_TAG_BITS)

	typedef union _EXHANDLE
	{
		struct
		{
			ULONG_PTR TagBits : HANDLE_TAG_BITS;
			ULONG_PTR Index : HANDLE_INDEX_BITS;
			ULONG_PTR KernelFlag : KERNEL_FLAG_BITS;
		};
		struct
		{
			ULONG_PTR TagBits2 : HANDLE_TAG_BITS;
			ULONG_PTR LowIndex : HANDLE_LOW_BITS;
			ULONG_PTR MidIndex : HANDLE_HIGH_BITS;
			ULONG_PTR HighIndex : HANDLE_HIGH_BITS;
			ULONG_PTR KernelFlag2 : KERNEL_FLAG_BITS;
		};
		HANDLE GenericHandleOverlay;
		ULONG_PTR Value;
		ULONG AsULONG;
	} EXHANDLE, * PEXHANDLE;

	typedef struct _HANDLE_TABLE_ENTRY
	{
		union
		{
			LONG_PTR VolatileLowValue;
			LONG_PTR LowValue;
			PVOID InfoTable;
			LONG_PTR RefCountField;
			struct
			{
				ULONG_PTR Unlocked : 1;
				ULONG_PTR RefCnt : 16;
				ULONG_PTR Attributes : 3;
				ULONG_PTR ObjectPointerBits : 44;
			};
		};
		union
		{
			LONG_PTR HighValue;
			struct _HANDLE_TABLE_ENTRY* NextFreeHandleEntry;
			EXHANDLE LeafHandleValue;
			struct
			{
				ULONG32 GrantedAccessBits : 25;
				ULONG32 NoRightsUpgrade : 1;
				ULONG32 Spare1 : 6;
			};
			ULONG32 Spare2;
		};
	} HANDLE_TABLE_ENTRY, * PHANDLE_TABLE_ENTRY;

	typedef struct _HANDLE_TABLE_FREE_LIST
	{
		ULONG_PTR FreeListLock;
		PHANDLE_TABLE_ENTRY FirstFreeHandleEntry;
		PHANDLE_TABLE_ENTRY lastFreeHandleEntry;
		LONG32 HandleCount;
		ULONG32 HighWaterMark;
		ULONG32 Reserved[8];
	} HANDLE_TABLE_FREE_LIST, * PHANDLE_TABLE_FREE_LIST;

	typedef struct _HANDLE_TABLE
	{
		ULONG32 NextHandleNeedingPool;
		LONG32 ExtraInfoPages;
		ULONG_PTR TableCode;
		PEPROCESS QuotaProcess;
		LIST_ENTRY HandleTableList;
		ULONG32 UniqueProcessId;
		union
		{
			ULONG32 Flags;
			struct
			{
				BOOLEAN StrictFIFO : 1;
				BOOLEAN EnableHandleExceptions : 1;
				BOOLEAN Rundown : 1;
				BOOLEAN Duplicated : 1;
				BOOLEAN RaiseUMExceptionOnInvalidHandleClose : 1;
			};
		};
		ULONG_PTR HandleContentionEvent;
		ULONG_PTR HandleTableLock;
		union
		{
			HANDLE_TABLE_FREE_LIST FreeLists[1];
			BOOLEAN ActualEntry[32];
		};
		PVOID DebugInfo;
	} HANDLE_TABLE, * PHANDLE_TABLE;
}