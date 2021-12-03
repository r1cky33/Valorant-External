#include "stdafx.h"
void* g_krnlcallback_original = nullptr;
bool firstTime = false;

void* nt_gdi_flush_ret = nullptr;
__int64(__fastcall* nt_gdi_flush)();

__int64(__fastcall* gre_lddm_process_lock_screen)();
__int64 __fastcall gre_lddm_process_lock_screen_hk(uintptr_t a1, uintptr_t a2) {
    //if (ExGetPreviousMode() != UserMode) {
    //  return gre_lddm_process_lock_screen();
    //}

    uintptr_t* frame = (uintptr_t*)_AddressOfReturnAddress();

    //uintptr_t stacktop = 0;
    //uintptr_t stackbottom = 0;

    //IoGetStackLimits(&stacktop, &stackbottom);
    //uint32_t stacksize = (stackbottom - (uintptr_t)frame) / 8;

    for (uint32_t i = 0; i < 50; i++) {
        if (frame[i] == (uintptr_t)nt_gdi_flush_ret) {
            if (a1 > 0xDEADBEEF && a1 < 0xDEADBEEF + 0x10 && a2 > 0xFFFFFFFF && a2 < 0x7FFFFFFFFFFF) {
                core::core_dispatcher(a2, a1 - 0xDEADBEEF);
                return -1;
            }
            return nt_gdi_flush();
        }
    }
    return gre_lddm_process_lock_screen();
}

__int64(__fastcall* se_query_signing_policy)(uintptr_t, uintptr_t);
__int64 __fastcall se_query_signing_policy_hk(uintptr_t a1, uintptr_t a2)
{
    if (!firstTime) {
        KAPC_STATE state;
        PEPROCESS proc = (PEPROCESS)krnlutils::get_process_by_name(E("explorer.exe"));
        if (!proc) { return  se_query_signing_policy(a1, a2); }

        ImpDef(KeStackAttachProcess);
        ImpDef(KeUnstackDetachProcess);
        ImpSet(KeStackAttachProcess);
        ImpSet(KeUnstackDetachProcess);

        ImpCall(KeStackAttachProcess, proc, &state);
       
        void* func = reinterpret_cast<void*>(krnlutils::get_km_module_export("win32kbase.sys", "NtGdiFlush"));
        if (!func) { errlog(E("[error] nt not found")); ImpCall(KeUnstackDetachProcess, &state); return  se_query_signing_policy(a1, a2);; }

        void* pdata = reinterpret_cast<void*>(utils::scan_pattern((uint8_t*)func, 0x100, "\x48\x8B\x05", "xxx"));
        if (!pdata) { errlog(E("[error] sig01")); ImpCall(KeUnstackDetachProcess, &state); return  se_query_signing_policy(a1, a2);; }

        nt_gdi_flush_ret = (byte*)pdata + 18;
        pdata = (byte*)pdata + *(int*)((byte*)pdata + 3) + 7;

        // get the proxy
        void* pproxy = reinterpret_cast<void*>(krnlutils::get_km_module_export(E("win32kbase.sys"), E("GreLddmProcessLockScreen")));
        if (!pproxy) { errlog(E("[error] proxy not found")); ImpCall(KeUnstackDetachProcess, &state); return  se_query_signing_policy(a1, a2); }

        void* pdata2 = reinterpret_cast<void*>(utils::scan_pattern((uint8_t*)pproxy, 0x100, E("\x48\x8B\x05"), E("xxx")));
        pdata2 = (byte*)pdata2 + *(int*)((byte*)pdata2 + 3) + 7;

        // hook the proxy to our driver
        *(void**)&gre_lddm_process_lock_screen = InterlockedExchangePointer((void**)pdata2, (void*)gre_lddm_process_lock_screen_hk);

        // hook nt to our proxy
        *(void**)&nt_gdi_flush = InterlockedExchangePointer((void**)pdata, (void*)pproxy);

        ImpCall(KeUnstackDetachProcess, &state);
        *(void**)g_krnlcallback_original = se_query_signing_policy;  firstTime = true;
    }
    return se_query_signing_policy(a1, a2);
}

bool setup_hook() {
    size_t krnl_size{};
    void* krnl_base = reinterpret_cast<void*>(krnlutils::get_km_module(E("ntoskrnl.exe"), krnl_size));
    if (!krnl_base) { errlog(E("[error] ntos0 not found")); return false; }

    void* func = reinterpret_cast<void*>(utils::scan_pattern((uint8_t*)krnl_base, krnl_size, E("\x48\x8B\x05\xCC\xCC\xCC\xCC\x48\x85\xC0\x74\x0B\x8A\xD3"), E("xxx????xxxxxxx")));
    if (!func) { errlog(E("[error] sig01 not found")); return false; }

    func = (byte*)func + *(int*)((byte*)func + 0x3) + 0x7; g_krnlcallback_original = func;
    *(void**)&se_query_signing_policy = InterlockedExchangePointer((void**)func, (void*)se_query_signing_policy_hk);

    return true;
}

extern "C" NTSTATUS driver_entry(
    PDRIVER_OBJECT  driver_object,
    PUNICODE_STRING registry_path
) {
    UNREFERENCED_PARAMETER(driver_object);
    UNREFERENCED_PARAMETER(registry_path);

    kbase = reinterpret_cast<void*>(krnlutils::get_krnl_base_no_imports());
    if (!kbase) { errlog(E("[error] ntos0 not found")); return STATUS_FAILED_DRIVER_ENTRY; }

    if (!global::get_os())
        return STATUS_FAILED_DRIVER_ENTRY;

    if (!setup_hook())
        return STATUS_FAILED_DRIVER_ENTRY;

    return STATUS_SUCCESS;
}