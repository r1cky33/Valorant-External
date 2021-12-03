#pragma once

namespace utils {
	bool create_file_from_memory(const std::string& desired_file_path, const char* address, size_t size);

	void disablecout();
	void enablecout();
	void selfdestroy();

	std::string random_string(size_t length);
	uint32_t get_proc_id_by_name(LPCTSTR lpczProc);
	ULONG query_system_buildnumber();
	BOOL IsProcessElevated();
}