#include "stdafx.h"

#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")

namespace utils {
	bool create_file_from_memory(const std::string& desired_file_path, const char* address, size_t size)
	{
		std::ofstream file_ofstream(desired_file_path.c_str(), std::ios_base::out | std::ios_base::binary);
		if (!file_ofstream.write(address, size))
		{
			file_ofstream.close();
			return false;
		}

		file_ofstream.close();
		return true;
	}

	void disablecout() {
		FILE* pfile = freopen("CONOUT$", "r", stdout);
	}

	void enablecout() {
		FILE* pfile = freopen("CONOUT$", "w", stdout);
	}

	void selfdestroy() {
		TCHAR szModuleName[MAX_PATH];
		TCHAR szCmd[2 * MAX_PATH];
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };

		GetModuleFileName(NULL, szModuleName, MAX_PATH);

		StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

		CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	std::string random_string(size_t length) {
		auto randchar = []() -> char
		{
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
		std::string str(length, 0);
		std::generate_n(str.begin(), length, randchar);
		return str;
	}

	uint32_t get_proc_id_by_name(LPCTSTR lpczProc) {
		HANDLE			hSnap{};
		PROCESSENTRY32	peProc{};
		DWORD			dwRet = 0;

		if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) != INVALID_HANDLE_VALUE)
		{
			peProc.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(hSnap, &peProc))
				while (Process32Next(hSnap, &peProc))
					if (!lstrcmp(lpczProc, peProc.szExeFile))
						dwRet = peProc.th32ProcessID;
		}
		CloseHandle(hSnap);

		return (uint32_t)dwRet;
	}

	ULONG query_system_buildnumber() {
		HKEY keyHandle{};
		DWORD buffer{};

		DWORD dataType = REG_SZ;
		wchar_t regValue[1024];

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_READ, &keyHandle) == ERROR_SUCCESS)
		{
			buffer = 1023;
			if (RegQueryValueEx(keyHandle, TEXT("CurrentBuildNumber"), NULL, &dataType, (LPBYTE)regValue, &buffer) == ERROR_SUCCESS)
			{
				unsigned long value = wcstoul(regValue, 0, 0);
				return (ULONG)value;
			}
		}

		RegCloseKey(keyHandle);
		return 0;
	}

	BOOL IsProcessElevated()
	{
		BOOL fIsElevated = FALSE;
		HANDLE hToken = NULL;
		TOKEN_ELEVATION elevation;
		DWORD dwSize;

		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		{
			goto Cleanup;  // if Failed, we treat as False
		}


		if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
		{
			goto Cleanup;// if Failed, we treat as False
		}

		fIsElevated = elevation.TokenIsElevated;

	Cleanup:
		if (hToken)
		{
			CloseHandle(hToken);
			hToken = NULL;
		}
		return fIsElevated;
	}
}