#pragma once

namespace auth {
#ifndef _AUTH_STATES
#define _AUTH_STATES
	enum auth_state {
		expired = 0,
		invalid_request,
		unknown,
		banned,
		valid
};
#endif

	LONG get_string_reg_key(HKEY hKey, const std::wstring& strValueName, std::wstring& strValue, const std::wstring& strDefaultValue);
	auth_state authenticate();

	std::string get_hwid();
	std::string get_license();

	size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data);
}