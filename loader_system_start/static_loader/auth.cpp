#include "stdafx.h"

// -------------- CURL DEPENDENCIE -------------------
#define CURL_STATICLIB
#pragma warning(disable:4996)

#pragma comment(lib, "libcurl_a.lib")

#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")

#include <curl.h>
// -------------- CURL DEPENDENCIE -------------------

namespace auth {

#define LOADER_VERSION _xor_("3.1.0").c_str()

	auth_state authenticate() {
		std::string get_l = _xor_("license=").c_str();
		get_l.append(get_license().c_str());
		std::string get_h = _xor_("&hwid=").c_str();
		get_h.append(get_hwid().c_str());
		std::string version = _xor_("&version=").c_str();
		version.append(LOADER_VERSION);

		std::string url = std::string(_xor_("http://project-chaos.xyz/auth_dayz/auth.php?").c_str()) + get_l + get_h + version;

		std::string removeables = " ";
		for (char c : removeables) {
			url.erase(std::remove(url.begin(), url.end(), c), url.end());
		}

		auto curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
			curl_easy_setopt(curl, CURLOPT_USERPWD, "user:pass");
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
			curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
			curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

			std::string response_string;
			std::string header_string;
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

			char* url;
			long response_code;
			double elapsed;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
			curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);

			curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			curl = NULL;

			if (!response_string.empty()) {
				std::string bin = crypt::hex2bin(response_string);
				std::string decoded = crypt::xorstr(bin, get_hwid());

				{
					//	handle non-pe-cases
					if (decoded.compare(std::string(_xor_("valid").c_str())) == 0) {
						return auth_state::valid;
					}

					if (decoded.compare(std::string(_xor_("expired").c_str())) == 0) {
						return auth_state::expired;
					}

					if (decoded.compare(std::string(_xor_("invalid_request").c_str())) == 0) {
						return auth_state::invalid_request;
					}

					if (decoded.compare(std::string(_xor_("banned").c_str())) == 0) {
						return auth_state::banned;
					}
				}
			}
			msg_box(_xor_("unknown error!").c_str(), _xor_("error").c_str());
		}
		return auth_state::unknown;
	}

	size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
		data->append((char*)ptr, size * nmemb);
		return size * nmemb;
	}

	LONG get_string_reg_key(HKEY hKey, const std::wstring& strValueName, std::wstring& strValue, const std::wstring& strDefaultValue)
	{
		strValue = strDefaultValue;
		WCHAR szBuffer[512];
		DWORD dwBufferSize = sizeof(szBuffer);
		ULONG nError;
		nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
		if (ERROR_SUCCESS == nError)
		{
			strValue = szBuffer;
		}
		return nError;
	}

	std::string get_hwid() {
		HKEY hKey;
		LONG lRes = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ, &hKey);
		bool bExistsAndSuccess(lRes == ERROR_SUCCESS);
		bool bDoesNotExistsSpecifically(lRes == ERROR_FILE_NOT_FOUND);
		std::wstring strValueOfBinDir;
		std::wstring strKeyDefaultValue;
		get_string_reg_key(hKey, _xor_(L"MachineGuid").c_str(), strValueOfBinDir, _xor_(L"bad").c_str());

		std::string hwid(strValueOfBinDir.begin(), strValueOfBinDir.end());
		return hwid;
	}

	std::string get_license() {
		std::string license;
		std::cout << _xor_("Enter Key:").c_str();
		std::cin >> license;
		return license;
	}
}