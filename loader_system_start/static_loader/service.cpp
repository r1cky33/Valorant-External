#include "stdafx.h"

#include "driver.h"

namespace service {
#define DRIVER_NAME _xor_("hsuifg.sys").c_str()

	bool set_group_load_order(byte tagid) {
		byte reg_bin_key[] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		LONG lReg{};
		HKEY hKey{};

		reg_bin_key[4] = tagid;

		lReg = RegCreateKeyEx(
			HKEY_LOCAL_MACHINE,
			_xor_(L"SYSTEM\\CurrentControlSet\\Control\\GroupOrderList").c_str(),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&hKey,
			NULL
		);

		if (lReg != ERROR_SUCCESS) {
			return false;
		}

		lReg = RegSetValueEx(
			hKey,
			_xor_(L"System Reserved").c_str(),
			NULL,
			REG_BINARY,
			(LPBYTE)&reg_bin_key,
			sizeof(reg_bin_key)
		);

		if (lReg != ERROR_SUCCESS) {
			return false;
		}
		RegCloseKey(hKey);
		return true;
	}

	bool disable_meltdown_mitigations() {
		HKEY hKey{};
		LONG lReg{};

		DWORD disable = 0x3;

		lReg = RegCreateKeyEx(
			HKEY_LOCAL_MACHINE,
			_xor_(L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management").c_str(),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&hKey,
			NULL
		);

		if (lReg != ERROR_SUCCESS) { return false; }

		lReg = RegSetValueEx(
			hKey,
			_xor_(L"FeatureSettingsOverride").c_str(),
			NULL,
			REG_DWORD,
			(LPBYTE)&disable,
			sizeof(disable)
		);

		if (lReg != ERROR_SUCCESS) { return false; }


		lReg = RegSetValueEx(
			hKey,
			_xor_(L"FeatureSettingsOverrideMask").c_str(),
			NULL,
			REG_DWORD,
			(LPBYTE)&disable,
			sizeof(disable)
		);

		if (lReg != ERROR_SUCCESS) { return false; }

		RegCloseKey(hKey);
		return true;
	}

	bool register_service(const std::string& driver_path)
	{
		const std::string driver_name = std::filesystem::path(driver_path).filename().string();
		const SC_HANDLE sc_manager_handle = OpenSCManager(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);

		if (!sc_manager_handle)
			return false;

		DWORD tagId{};
		std::string loadgroup(_xor_("System Reserved").c_str());
		SC_HANDLE service_handle = CreateServiceA(sc_manager_handle, driver_name.c_str(), driver_name.c_str(), 0xF01FF,
			SERVICE_KERNEL_DRIVER, SERVICE_SYSTEM_START, SERVICE_ERROR_IGNORE,
			driver_path.c_str(), loadgroup.c_str(), &tagId, nullptr, nullptr, nullptr);

		if (!service_handle)
		{
			service_handle = OpenServiceA(sc_manager_handle, driver_name.c_str(), SERVICE_START);

			if (!service_handle)
			{
				CloseServiceHandle(sc_manager_handle);
				return false;
			}
		}

		CloseServiceHandle(service_handle);
		CloseServiceHandle(sc_manager_handle);

		return set_group_load_order((byte)tagId);
	}

	bool delete_service(const std::string& driver_path) {
		const std::string driver_name = std::filesystem::path(driver_path).filename().string();
		const SC_HANDLE sc_manager_handle = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);

		if (!sc_manager_handle)
			return false;

		SC_HANDLE service_handle = OpenServiceA(sc_manager_handle, driver_name.c_str(), DELETE);
		if (!service_handle)
		{
			return false;
		}

		if (!DeleteService(service_handle)) {
			return false;
		}

		CloseServiceHandle(service_handle);

		return true;
	}

	bool install() {
		char windows_dir[MAX_PATH] = { 0 };
		const uint32_t get_temp_path_ret = (uint32_t)GetWindowsDirectoryA(windows_dir, sizeof(windows_dir));

		if (!windows_dir || get_temp_path_ret > MAX_PATH) { return false; }

		const std::string drv_path = std::string(windows_dir) + _xor_("\\System32\\drivers\\").c_str() + DRIVER_NAME;
		delete_service(drv_path);
		std::remove(drv_path.c_str());

		// decrypt driver the retard way
		//byte* decrypted = new byte[sizeof(drv_map)];
		//for (uint32_t i = 0; i < sizeof(drv_map); i++) {
		//	decrypted[i] = drv_map[i] - 8;
		//}

		if (!utils::create_file_from_memory(drv_path, reinterpret_cast<const char*>(drv_map), sizeof(drv_map)))
		{
			msg_box(_xor_("Service Error 1").c_str(), _xor_("error").c_str());
			return false;
		}

		//delete[] decrypted;

		if (!disable_meltdown_mitigations()) {
			msg_box(_xor_("Service Error 2").c_str(), _xor_("error").c_str());
			std::remove(drv_path.c_str());
			return false;
		}

		if (!register_service(drv_path))
		{
			msg_box(_xor_("Service Error 3 - Try restarting your system").c_str(), _xor_("error").c_str());
			std::remove(drv_path.c_str());
			return false;
		}

		return true;
	}
}