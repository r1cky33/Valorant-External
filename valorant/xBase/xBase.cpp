#include "stdafx.hpp"

int main()
{
    ::printf(_xor_("[+] ... start Valorant!\n").c_str());
    ::printf(_xor_("[+] ... press F12 to exit!\n").c_str());

    if (!core::core_init()) {
        ::printf(_xor_("[-] service not initialized...\n").c_str());
        ::printf(_xor_("[-] try restarting your system!\n").c_str());
        std::this_thread::sleep_for(std::chrono::seconds(7));
        return -1;
    }

    while (globals::target_proc::target_wnd_hwnd == nullptr) {
        globals::target_proc::target_wnd_hwnd = ::FindWindowA(0, _xor_("VALORANT  ").c_str());
    }

    ::printf(_xor_("[+] target window found!\n").c_str());

    uint32_t tid = ::GetWindowThreadProcessId(globals::target_proc::target_wnd_hwnd, (PDWORD)&globals::target_proc::target_process_id);

    if (!globals::target_proc::target_process_id ||
        !globals::target_proc::target_wnd_hwnd)
        return -2;

    return (srcns::init()) ? 1 : -3;
}