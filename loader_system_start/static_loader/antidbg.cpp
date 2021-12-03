#include "stdafx.h"

BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
	int length = GetWindowTextLength(hWnd);
	char* buffer = new char[length + 1];
	GetWindowTextA(hWnd, buffer, length + 1);
	std::string windowTitle(buffer);

	if (IsWindowVisible(hWnd) && length != 0) {
		if (windowTitle.find(std::string(_xor_("OLLYDBG").c_str())) != std::string::npos) {
			exit(-1);
		}
		if (windowTitle.find(std::string(_xor_("WinDbgFrameClass").c_str())) != std::string::npos) {
			exit(-1);
		}
		if (windowTitle.find(std::string(_xor_("IDA").c_str())) != std::string::npos) {
			exit(-1);
		}
		if (windowTitle.find(std::string(_xor_("Zeta Debugger").c_str())) != std::string::npos) {
			exit(-1);
		}
		if (windowTitle.find(std::string(_xor_("ObsidianGUI").c_str())) != std::string::npos) {
			exit(-1);
		}
		if (windowTitle.find(std::string(_xor_("Rock Debugger").c_str())) != std::string::npos) {
			exit(-1);
		}
		if (windowTitle.find(std::string(_xor_("x64dbg").c_str())) != std::string::npos) {
			exit(-1);
		}
	}
	return TRUE;
}

namespace antidbg {
	byte shellcode[] = { 0x48, 0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD0 };

	void patch_dbg_ui_remote_breakin() {
		HANDLE hProcess = GetCurrentProcess();

		HMODULE hMod = GetModuleHandleW(_xor_(L"ntdll.dll").c_str());
		if (hMod) {
			FARPROC func_DbgUiRemoteBreakin = GetProcAddress(hMod, _xor_("DbgUiRemoteBreakin").c_str());

			*(uint64_t*)&shellcode[2] = (uint64_t)&ExitProcess;
			WriteProcessMemory(hProcess, func_DbgUiRemoteBreakin, &shellcode, sizeof(shellcode), NULL);
		}
	}

	void wnd_check() {
		while (true) {
			EnumWindows(enumWindowCallback, NULL);
			Sleep(5000);
		}
	}

	void init() {
		// closes on attach
		patch_dbg_ui_remote_breakin();

		// check open windows
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)wnd_check, 0, 0, 0);
	}
}