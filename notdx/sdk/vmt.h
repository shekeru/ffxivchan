#pragma once
#include "base.h"
#include <psapi.h>
#include <detours.h>
// Guard Class
namespace detail
{
	class protect_guard
	{
	public:
		protect_guard(void* base, size_t len, uint32_t flags)
		{
			_base = base;
			_length = len;
			if (!VirtualProtect(base, len, flags, (PDWORD)&_old))
				throw std::runtime_error("Failed to protect region.");
		}
		~protect_guard()
		{
			VirtualProtect(_base, _length, _old, (PDWORD)&_old);
		}

	private:
		void* _base;
		size_t _length;
		uint32_t _old;
	};
	inline bool validate(uintptr_t memory) {
		if (!memory || memory > 0x7fffffffffff) return false;
		MEMORY_BASIC_INFORMATION info; VirtualQuery(LPCVOID(memory),
			&info, sizeof(MEMORY_BASIC_INFORMATION));
		return info.AllocationProtect & PAGE_EXECUTE_WRITECOPY;
	};
};
// VMT Class
class VMT
{
public:
	uintptr_t* vmt;
	uintptr_t* sdk_vmt = nullptr;
	uintptr_t** object = nullptr;
	std::map<uintptr_t, size_t> map;
	size_t method_count = 0;
	VMT(void* instance)
	{
		object = reinterpret_cast<uintptr_t**>(instance);
		sdk_vmt = *object; while (detail::validate(sdk_vmt[method_count]))
			method_count++; vmt = new uintptr_t[method_count + 1]();
		memcpy(&vmt[1], sdk_vmt, sizeof(uintptr_t) * method_count);
		printf("vm Total: %p => %d\n", object, method_count);
	}
	template <typename func>
	void HookVM(func ref, size_t index)
	{
		vmt[index + 1] = reinterpret_cast<uintptr_t>(ref);
		map[(uintptr_t)ref] = index;
	}
	template<typename Fn>
	auto GetOriginalMethod(Fn &ref, size_t index)
	{
		return reinterpret_cast<decltype(&ref)>(sdk_vmt[index]);
	}
	template<typename Fn>
	auto GetOriginalMethod(Fn &ref)
	{
		return GetOriginalMethod(ref, map[(uintptr_t)ref]);
	}
	void ReleaseVMT()
	{
		auto guard = detail::protect_guard{ object,
			sizeof(uintptr_t), PAGE_READWRITE };
		if (object && *object && sdk_vmt)
			*this->object = sdk_vmt;
	}
	void ApplyVMT()
	{
		auto guard = detail::protect_guard{ object,
			sizeof(uintptr_t), PAGE_READWRITE };
		*vmt = sdk_vmt[-1]; *object = &vmt[1];
	}
	~VMT() {
		ReleaseVMT();
		delete[] vmt;
	}
	void CheckVMT(int last_known) {
		if (method_count != last_known)
			printf("<VMT ERROR>, for _interface_ with x of y\n");
	}
public:
	INSTANCE(VMT, Factory);
	INSTANCE(VMT, SwapChain);
}; class MemorySystem {
public:
	MODULEINFO baseModule;
	MemorySystem(const char* exe_name) {
		GetModuleInformation(GetCurrentProcess(),
			GetModuleHandle(exe_name), &baseModule, sizeof(baseModule));
		printf("ffxiv Executable Start -> %p\n", baseModule.lpBaseOfDll);
	}
	uintptr_t GetLocation(const char* signature) {
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		auto sizeOfImage = baseModule.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(baseModule.lpBaseOfDll);
		auto s = patternBytes.size(); auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i) {
			bool found = true;
			for (auto j = 0ul; j < s; ++j) {
				if (scanBytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found) {
				return uintptr_t(scanBytes + i);
			}
		}
		return NULL;
	}
	IntPtr ScanPattern(const char* signature, int size, int extra = 0)
	{
		auto offset = (int*)(GetLocation(signature) + size);
		return IntPtr(*offset + uintptr_t(offset) + 4 + extra);
	}; void DetourAll();
}; inline MemorySystem* game;