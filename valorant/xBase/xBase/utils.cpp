#include "stdafx.hpp"

std::string utils::randomstring(int len)
{
	char alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
						  'h', 'i', 'j', 'k', 'l', 'm', 'n',
						  'o', 'p', 'q', 'r', 's', 't', 'u',
						  'v', 'w', 'x', 'y', 'z' };

	std::string res = "";
	for (int i = 0; i < len; i++)
		res = res + alphabet[rand() % 27];

	return res;
}

std::string utils::string_to_utf8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

void utils::hide_from_taskbar(HWND hwnd) {
	ITaskbarList* pTaskList = NULL;
	HRESULT initRet = CoInitialize(NULL);
	HRESULT createRet = CoCreateInstance(CLSID_TaskbarList,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskbarList,
		(LPVOID*)&pTaskList);

	if (createRet == S_OK)
	{

		pTaskList->DeleteTab(hwnd);

		pTaskList->Release();
	}

	CoUninitialize();
}

bool utils::is_valid_addr(uint64_t addr) {
	if (addr > 0x1000 && addr < 0x7FFFFFFFFFFF)
		return true;
	else
		return false;
}

std::uintptr_t utils::scanPattern(std::uint8_t* base, const std::size_t size, char* pattern, char* mask) {
	const auto patternSize = strlen(mask);

	for (std::size_t i = {}; i < size - patternSize; i++)
	{
		for (std::size_t j = {}; j < patternSize; j++)
		{
			if (mask[j] != '?' && *reinterpret_cast<std::uint8_t*>(base + i + j) != static_cast<std::uint8_t>(pattern[j]))
				break;

			if (j == patternSize - 1)
				return reinterpret_cast<std::uintptr_t>(base) + i;
		}
	}

	return {};
}

DWORD utils::get_proc_id_by_name(LPCTSTR lpczProc)
{
	HANDLE			hSnap;
	PROCESSENTRY32	peProc;
	DWORD			dwRet = -1;

	if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) != INVALID_HANDLE_VALUE)
	{
		peProc.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnap, &peProc))
			while (Process32Next(hSnap, &peProc))
				if (!lstrcmp(lpczProc, peProc.szExeFile))
					dwRet = peProc.th32ProcessID;
	}
	CloseHandle(hSnap);

	return dwRet;
}

bool utils::get_process_threads(uint32_t dwOwnerPID, std::list<uint32_t>& thread_ids) {
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);

	te32.dwSize = sizeof(THREADENTRY32);

	if (!Thread32First(hThreadSnap, &te32))
	{
		CloseHandle(hThreadSnap);
		return false;
	}

	do
	{
		if (te32.th32OwnerProcessID == dwOwnerPID)
		{
			thread_ids.push_back(te32.th32ThreadID);
		}
	} while (Thread32Next(hThreadSnap, &te32));

	CloseHandle(hThreadSnap);
	return true;
}

wchar_t* utils::getwc(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

bool utils::IsBitSet(byte b, int pos) { return (b & (1 << pos)) != 0; }

void utils::seprivilege()
{
	TOKEN_PRIVILEGES NewState;
	LUID luid;
	HANDLE hToken;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid = luid;
	NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &NewState, sizeof(NewState), NULL, NULL);

	CloseHandle(hToken);

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("Execute o programa como Administrador.");
		Sleep(5000);
		ExitProcess(0);
	}
}