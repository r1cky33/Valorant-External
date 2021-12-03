#include "stdafx.hpp"

#include <assert.h>

#define PAGE_SIZE 4096

namespace magic {
	//offsets for auto update
	uintptr_t magic_o_read_proxy = 0;
	uintptr_t magic_o_write_proxy = 0;
	uintptr_t magic_o_datahk = 0;
	uintptr_t magic_original_func = 0;

	BYTE magic[] = { 0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53, 0x9C, 0x56, 0x57, 0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,0x9C, 0x56, 0x57, 0x55, 0x50, 0x53, 0x51, 0x52, 0x54, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53, 0xFF, 0xFF, 0xFF, 0xE0 };


	uint64_t presults = 0;

	void update_shell(uintptr_t decrypted_world, uintptr_t pcodecave, uintptr_t base, uintptr_t pentitycache) {
		for (int i = 0; i <= sizeof(magic) - sizeof(UINT64); i++)
		{
			const auto pShell = (UINT64*)(magic + i);
			if (*pShell == 0xAAAAAAAAAAAAAAAA)
			{
				*pShell = decrypted_world;	// puworld
			}
			if (*pShell == 0xBBBBBBBBBBBBBBBB)
			{
				*pShell = presults;		// allocated memory
			}
			if (*pShell == 0xCCCCCCCCCCCCCCCC)
			{
				*pShell = base + magic_o_read_proxy; // mov rax, [rax] => ret
			}
			if (*pShell == 0xFFFFFFFFFFFFFFFF)
			{
				*pShell = magic_original_func;	// original function of hook
			}
			if (*pShell == 0xDDDDDDDDDDDDDDDD)
			{
				*pShell = pentitycache; 
			}
			if (*pShell == 0x9999999999999999)
			{
				*pShell = magic_o_write_proxy;
			}
			if (*(UINT32*)pShell == 0xDDDD)
			{
				*(UINT32*)pShell = 0x38;
			}
			if (*(UINT32*)pShell == 0xEEEE)
			{
				*(UINT32*)pShell = 0x1a8;
			}
			if (*(UINT32*)pShell == 0xCAFE)
			{
				*(UINT32*)pShell = 0xa0;
			}
			if (*(UINT32*)pShell == 0xBABE)
			{
				*(UINT32*)pShell = 0xa8;
			}
		}
	}

	template<class T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		assert(!(hi < lo));
		return (v < lo) ? lo : (hi < v) ? hi : v;
	}

	uint64_t find_hidden_module() {
		uint64_t base = core::get_process_base_by_id(globals::target_proc::target_process_id);
		DWORD valorant_pid = (DWORD)globals::target_proc::target_process_id;

		auto query = [&valorant_pid](uint64_t address, void* buffer) {
			static auto process = OpenProcess(PROCESS_QUERY_INFORMATION, false, valorant_pid);
			return VirtualQueryEx(process, reinterpret_cast<void*>(address), (PMEMORY_BASIC_INFORMATION)buffer, sizeof(MEMORY_BASIC_INFORMATION));
		};

		IMAGE_DOS_HEADER dos = core::read<IMAGE_DOS_HEADER>(globals::target_proc::target_process_id, base);
		IMAGE_NT_HEADERS nt = core::read<IMAGE_NT_HEADERS>(globals::target_proc::target_process_id, base + dos.e_lfanew);

		SYSTEM_INFO sys_info = { 0 };
		GetSystemInfo(&sys_info);
		uint64_t region_start = reinterpret_cast<uint64_t>(sys_info.lpMinimumApplicationAddress), region_end = reinterpret_cast<uint64_t>(sys_info.lpMaximumApplicationAddress);

		clock_t tick = clock();
		MEMORY_BASIC_INFORMATION mem_info = {};

		uint64_t hidden_module = NULL;

		do {
			if (query(region_start, &mem_info)) {
				if ((mem_info.State & ~MEM_FREE) && (mem_info.Type & MEM_MAPPED)) {
					uint64_t region_base = reinterpret_cast<uint64_t>(mem_info.BaseAddress); region_base -= reinterpret_cast<uint64_t>(mem_info.BaseAddress) % PAGE_SIZE;
					for (int i = 0; i < clamp(mem_info.RegionSize / PAGE_SIZE, 0ull, 1000ull); i++) {
						char output[2] = {};

						core::mem_cpy(globals::target_proc::target_process_id, region_base, GetCurrentProcessId(), (uintptr_t)output, sizeof(output));

						if (!memcmp(reinterpret_cast<void*>(output), "MZ", sizeof(output))) {
							IMAGE_DOS_HEADER dos_ = {};
							IMAGE_NT_HEADERS nt_ = {};

							dos_ = core::read<IMAGE_DOS_HEADER>(globals::target_proc::target_process_id, region_base, sizeof(dos));
							nt_ = core::read<IMAGE_NT_HEADERS>(globals::target_proc::target_process_id, region_base + dos.e_lfanew, sizeof(nt));

							bool is_exe = nt.FileHeader.Characteristics & ~(IMAGE_FILE_DLL | IMAGE_FILE_SYSTEM);
							if (dos.e_magic == IMAGE_DOS_SIGNATURE && nt.Signature == IMAGE_NT_SIGNATURE && is_exe && nt.OptionalHeader.CheckSum == nt_.OptionalHeader.CheckSum && region_base != base) {
								hidden_module = region_base;
								break;
							}
						}
						region_base += PAGE_SIZE;
					}

					if (hidden_module)
						break;
				}
			}

			region_start += mem_info.RegionSize;
		} while (region_start <= region_end);

		return hidden_module;
	}

	uint64_t search_codecave(uint32_t& size) {
		SYSTEM_INFO sys_info = { 0 };
		::GetSystemInfo(&sys_info);
		uint64_t region_start = reinterpret_cast<uint64_t>(sys_info.lpMinimumApplicationAddress), region_end = reinterpret_cast<uint64_t>(sys_info.lpMaximumApplicationAddress);

		uint32_t tmp = globals::target_proc::target_process_id;

		auto query = [&tmp](uint64_t address, void* buffer) {
			static auto process = ::OpenProcess(PROCESS_QUERY_INFORMATION, false, tmp);
			return ::VirtualQueryEx(process, reinterpret_cast<void*>(address), (PMEMORY_BASIC_INFORMATION)buffer, sizeof(MEMORY_BASIC_INFORMATION));
		};

		MEMORY_BASIC_INFORMATION mbi = {};

		uintptr_t ctr = 0;

		do {
			if (query(region_start, &mbi)) {
				if (mbi.AllocationProtect == PAGE_EXECUTE_READWRITE && mbi.State == MEM_COMMIT && mbi.Type == MEM_PRIVATE) {
					uint64_t region_base = reinterpret_cast<uint64_t>(mbi.BaseAddress);
					uint32_t region_size = mbi.RegionSize;

					::printf("> analyzing RWX page found at: 0x%p     size: 0x%x  \n", region_base, region_size);

					byte* buffer = (byte*)::VirtualAlloc(nullptr, region_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
					::ZeroMemory(buffer, region_size);

					core::mem_cpy(tmp, region_base, ::GetCurrentProcessId(), (uintptr_t)buffer, region_size);

					bool skip = false;
					for (uint32_t i = 0; i < region_size; i++) {
						if (*(byte*)((uintptr_t)buffer + i) != (byte)0 || ctr < 3) {
							ctr++;
							skip = true;
						}
					}

					::VirtualFree(buffer, region_size, MEM_RELEASE);

					if (!skip) {
						size = region_size;
						return region_base;
					}
				}
			}

			region_start += mbi.RegionSize;
		} while (region_start <= region_end);
	}

	bool auto_update(uintptr_t& worldcrypt_key, uintptr_t& worldcrypt_state) {
		//uintptr_t hidden_module = find_hidden_module();
		//if (!hidden_module) { printf("[#] hidden module not found\n"); return false; }

		//printf("[+] found hidden module: 0x%p\n", hidden_module);

		//uint64_t base = hidden_module;

		uint64_t base = globals::target_proc::target_process_base;
		DWORD valorant_pid = (DWORD)globals::target_proc::target_process_id;

		IMAGE_DOS_HEADER dos = core::read<IMAGE_DOS_HEADER>(globals::target_proc::target_process_id, base);
		IMAGE_NT_HEADERS nt = core::read<IMAGE_NT_HEADERS>(globals::target_proc::target_process_id, base + dos.e_lfanew);
		if (!nt.OptionalHeader.SizeOfCode) {
			::printf("[-] ERROR reading headers!\n");
			return false;
		}

		::printf("[-] size of code: 0x%p\n", nt.OptionalHeader.SizeOfCode);

		//really really really ghetto
		uintptr_t code_start = base + 0x1000;

		//void* codebuffer = malloc((size_t)(nt.OptionalHeader.SizeOfCode));
		byte* codebuffer = (byte*)::VirtualAlloc(nullptr, nt.OptionalHeader.SizeOfCode, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		::ZeroMemory(codebuffer, nt.OptionalHeader.SizeOfCode);
		if (!codebuffer) {
			::printf("[-] ERROR allocating memory for gamecpy!\n");
			return false;
		}
		//core::virtual_protect(globals::target_proc::target_process_id, code_start, PAGE_READWRITE, nt.OptionalHeader.SizeOfCode);
		
		for (uint32_t page = 0x0; page < nt.OptionalHeader.SizeOfCode; page += 0x1000) {
			core::mem_cpy(globals::target_proc::target_process_id, (uintptr_t)(code_start + page), GetCurrentProcessId(), (uintptr_t)((uintptr_t)codebuffer + page), (size_t)0x1000);
		}

		printf("[#] copied game\n");

		uintptr_t virtual_read_proxy = utils::scanPattern((uint8_t*)codebuffer, nt.OptionalHeader.SizeOfCode, (char*)"\x48\x8B\x00\xC3\x33", (char*)"xxxxx");
		if (!virtual_read_proxy) { printf("[#] virt read proxy not found\n"); return false; }
		magic_o_read_proxy = code_start + (virtual_read_proxy - (uintptr_t)codebuffer) - base;

		uintptr_t virtual_write_proxy = utils::scanPattern((uint8_t*)codebuffer, nt.OptionalHeader.SizeOfCode, (char*)"\x48\x89\x01\xC3", (char*)"xxxx");
		if (!virtual_write_proxy) { printf("[#] virt write proxy not found\n");  return false; }
		magic_o_write_proxy = code_start + (virtual_write_proxy - (uintptr_t)codebuffer) - base;

		uintptr_t virtual_hk_function = utils::scanPattern((uint8_t*)codebuffer, nt.OptionalHeader.SizeOfCode, (char*)"\x48\x8B\x05\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x4C\x8B\xE8\x48\x8B\x1D\x00\x00\x00\x00\x48\x89\x5D\xFF", (char*)"xxx????xx????xxxxxx????xxxx");
		if (!virtual_hk_function) { printf("[#] virt hook func not found\n"); return false; }

		uint32_t o_datahk = core::read<uint32_t>(globals::target_proc::target_process_id, globals::target_proc::target_process_base + 0x1000 + (virtual_hk_function - (uintptr_t)codebuffer) + 0x3);
		magic_o_datahk = 0x1000 + (virtual_hk_function - (uintptr_t)codebuffer) + o_datahk + 0x7;

		magic_original_func = core::read<uint64_t>(globals::target_proc::target_process_id, globals::target_proc::target_process_base + magic_o_datahk);
		std::cout << "[#] reading original func from " << std::hex << (uintptr_t)(globals::target_proc::target_process_base + magic_o_datahk) << std::endl;
		uintptr_t uworld_decrypt = utils::scanPattern((uint8_t*)codebuffer, nt.OptionalHeader.SizeOfCode, (char*)"\x48\x8D\x05\x00\x00\x00\x00\x45\x8B\xD3\x4E\x8B\x04\xD8\xB8\x25\x49\x92\x24\x41\xF7\xE3", (char*)"xxx????xxxxxxxxxxxxxxx");
		if (!uworld_decrypt) { printf("[#] uworld pattern not found\n"); return false; }

		uintptr_t real_decrypt = globals::target_proc::target_process_base + 0x1000 + (uworld_decrypt - (uintptr_t)codebuffer);
		uint32_t data_offset = core::read<uint32_t>(globals::target_proc::target_process_id, real_decrypt + 0x3);

		worldcrypt_state = real_decrypt + data_offset + 0x7;
		worldcrypt_key = worldcrypt_state + 0x38;

		::VirtualFree(codebuffer, (size_t)(nt.OptionalHeader.SizeOfCode), MEM_RELEASE);

		return true;
	}

	bool magic_scan(uintptr_t& worldcrypt_key, uintptr_t& worldcrypt_state) {
		for (int i = 0; i < 10; ++i) {
			if (!auto_update(worldcrypt_key, worldcrypt_state)) {
				Sleep(2000);
				continue;
			}

			std::cout << "[#] magic_o_read_proxy: 0x" << magic_o_read_proxy << std::endl;
			std::cout << "[#] magic_o_write_proxy: 0x" << magic_o_write_proxy << std::endl;
			std::cout << "[#] magic_o_datahk: 0x" << magic_o_datahk << std::endl;
			std::cout << "[#] magic_original_func: 0x" << magic_original_func << std::endl;
			std::cout << "[#] key: 0x" << worldcrypt_key << std::endl;
			std::cout << "[#] state: 0x" << worldcrypt_state << std::endl;

			//auto update
			if (magic_o_read_proxy && magic_o_write_proxy && magic_o_datahk && magic_original_func) {
				if (utils::is_valid_addr(worldcrypt_key) && utils::is_valid_addr(worldcrypt_state)) {
					return true;
				}
			}
			Sleep(1000);
		}
		return false;
	}

	const char* convert_file_size(uint64_t bytes)
	{
		char* suffix[] = { (char*)"B", (char*)"KB", (char*)"MB", (char*)"GB", (char*)"TB" };
		char length = sizeof(suffix) / sizeof(suffix[0]);

		int i = 0;
		double dblBytes = bytes;

		if (bytes > 1024) {
			for (i = 0; (bytes / 1024) > 0 && i < length - 1; i++, bytes /= 1024)
				dblBytes = bytes / 1024.0;
		}

		static char output[200];
		sprintf_s(output, "%.02lf %s", dblBytes, suffix[i]);
		return output;
	}

	bool write_shell(uint64_t decrypted_world, uint64_t base, uintptr_t& pentitycache /*where shellcode stores actors*/, uintptr_t& plocalproxy /*where shellcode stores lplayer info*/) {
		presults = core::virtual_alloc(globals::target_proc::target_process_id, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, 0x100);
		if (!presults) { printf(_xor_("[#] failed to find position for results").c_str()); return false; }
		plocalproxy = presults;

		printf(_xor_("[#] address to write results: 0x%p\n").c_str(), presults);

		//uint32_t codecave_size = 0;
		//uintptr_t pcodecave = search_codecave(codecave_size);

		//::printf("> found rwx codecave at: 0x%p		with size: 0x%x\n", pcodecave, codecave_size);

		//uintptr_t pcodecave = core::virtual_alloc(globals::target_proc::target_process_id, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READ, sizeof(magic));
		//if (!utils::is_valid_addr(pcodecave)) { printf(_xor_("[dbg] no codecave was found :-(: 0x%p\n").c_str(), pcodecave); return false; }

		uintptr_t pcodecave = 0;
		uint32_t size = 0;
		if(!core::get_um_module(globals::target_proc::target_process_id, "libGLESv2.dll", pcodecave, size)) {
			::printf("[-] failed to get libGLESv2.dll\n");
			return false;
		}

		pcodecave = pcodecave + 0x2de3f0 + 0x1000;

		uintptr_t _pentitycache = core::virtual_alloc(globals::target_proc::target_process_id, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, 0xfffff);
		if (!_pentitycache) { printf(_xor_("[#] failed to alloc entitycache!\n").c_str()); return false; }

		pentitycache = _pentitycache;

		printf(_xor_("[#] entitycache at: 0x%p\n").c_str(), _pentitycache);
		printf(_xor_("[#] pcodecave at: 0x%p\n").c_str(), pcodecave);

		update_shell(decrypted_world, pcodecave, base, _pentitycache);

		if (!core::write_to_readonly<void>(globals::target_proc::target_process_id, (uintptr_t)magic, pcodecave, sizeof(magic))) { return false; }
		//system("pause");

		core::write<uintptr_t>(globals::target_proc::target_process_id, (uintptr_t)&pcodecave, base + magic_o_datahk, sizeof(uintptr_t));
	}

	HijackState read_results() {
		HijackState state = { 0 };
		state = core::read<HijackState>(globals::target_proc::target_process_id, presults);
		return state;
	}
}