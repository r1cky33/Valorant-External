#pragma once

namespace utils {
	uintptr_t scan_pattern(uint8_t* base, 
		const size_t size,
		char* pattern, 
		char* mask);

	void* get_win32(PETHREAD ethr);
	void set_win32(PETHREAD ethr, void* new_, void* buffer);
	byte get_apc_index(PETHREAD ethr);
	void set_apc_index(PETHREAD ethr, byte new_, void* buffer);
	KPROCESSOR_MODE ke_set_previous_mode(KPROCESSOR_MODE mode);

	void sys_delay(ULONG timeout);

	// highminded noimport shit that im too dumb for

	template <typename Type>
	__forceinline Type EPtr(Type Ptr) {
		auto Key = (ULONG64)SharedUserData->Cookie *
			SharedUserData->Cookie *
			SharedUserData->Cookie *
			SharedUserData->Cookie;
		return (Type)((ULONG64)Ptr ^ Key);
	}

	template <typename StrType, typename StrType2>
	__forceinline bool StrICmp(StrType Str, StrType2 InStr, bool Two)
	{
#define ToLower(Char) ((Char >= 'A' && Char <= 'Z') ? (Char + 32) : Char)

		if (!Str || !InStr)
			return false;

		wchar_t c1, c2; do {
			c1 = *Str++; c2 = *InStr++;
			c1 = ToLower(c1); c2 = ToLower(c2);
			if (!c1 && (Two ? !c2 : 1))
				return true;
		} while (c1 == c2);

		return false;
	}

	// physical shit
	PVOID GetProcessBaseAddress(int pid);
	DWORD GetUserDirectoryTableBaseOffset();
	ULONG_PTR GetProcessCr3(PEPROCESS pProcess);
	ULONG_PTR GetKernelDirBase();
	NTSTATUS ReadVirtual(uint64_t dirbase, uint64_t address, uint8_t* buffer, SIZE_T size, SIZE_T* read);
	NTSTATUS WriteVirtual(uint64_t dirbase, uint64_t address, uint8_t* buffer, SIZE_T size, SIZE_T* written);
	NTSTATUS ReadPhysicalAddress(PVOID TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesRead);
	NTSTATUS WritePhysicalAddress(PVOID TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesWritten);
	uint64_t TranslateLinearAddress(uint64_t directoryTableBase, uint64_t virtualAddress);
	NTSTATUS ReadProcessMemory(int pid, PVOID Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* read);
	NTSTATUS WriteProcessMemory(int pid, PVOID Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* written);

}
