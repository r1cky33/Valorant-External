#include "stdafx.hpp"

int main()
{
    ::printf("[+] ... start notepad!\n");

    if (!core::core_init()) {
        ::printf("[+] kernelmode not initialized!\n");
        return -1;
    }

    while (globals::target_proc::target_wnd_hwnd == nullptr) {
        globals::target_proc::target_wnd_hwnd = ::FindWindowA("Notepad", 0);
    }

    ::printf("[+] target window found!\n");

    uint32_t tid = ::GetWindowThreadProcessId(globals::target_proc::target_wnd_hwnd, (PDWORD)&globals::target_proc::target_process_id);

    if (!globals::target_proc::target_process_id ||
        !globals::target_proc::target_wnd_hwnd)
        return -2;

    return (srcns::init()) ? 1 : -3;
}