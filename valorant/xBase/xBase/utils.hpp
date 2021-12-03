#pragma once

#include <list>

namespace utils {
	std::string		randomstring(int len);
	std::string		string_to_utf8(const std::string& str);
	void			hide_from_taskbar(HWND hwnd);
	bool			is_valid_addr(uint64_t addr);
	std::uintptr_t	scanPattern(std::uint8_t* base, const std::size_t size, char* pattern, char* mask);
	DWORD			get_proc_id_by_name(LPCTSTR lpczProc);
	void			seprivilege();
	
	bool get_process_threads(uint32_t dwOwnerPID, std::list<uint32_t>& thread_ids);
	bool IsBitSet(byte b, int pos);

	wchar_t* getwc(const char* c);
}
