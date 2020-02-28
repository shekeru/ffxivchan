#pragma once
#include "base.h"
#include <psapi.h>
// Guard Class
namespace detail {
	class protect_guard {
	public:
		protect_guard(void* base, size_t len, uint32_t flags)
		{
			_base = base; _length = len;
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
	}; inline bool validate(uintptr_t memory) {
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
	MemorySystem(const char* exe_name); void StackTrace();
	template<typename TYPE = int>
	TYPE* GetLocation(const char* signature, int start = 0) {
		local pattern_to_byte = [](const char* pattern) {
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
			}; return bytes;
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
			} if (found) {
				return (TYPE*)(scanBytes + i + start);
			}
		}; return (TYPE*)printf("SCAN FAILURE: %s\n", signature);
	}; IntPtr ScanPattern(const char* signature, int size, int extra = 0) {
		auto offset = GetLocation<int>(signature, size); return 
			IntPtr(*offset + uintptr_t(offset) + 4 + extra);
	}; void DetourAll();
}; inline MemorySystem* game;
// Move Somewhere Else?
#include <nlohmann/json.hpp>
using json = nlohmann::json;
// SDK Namespace
#define pattern inline const char*
// Memory Patterns
namespace Offsets {
	pattern QUEST_2 =
		"48 8d 05 ? ? ? ? 48 8d 0c db 48";
	pattern HEAP_HANDLE =
		"48 8b 0d ? ? ? ? 4c 8b c3 ba";
	pattern SETS_REAL =
		"e8 ? ? ? ? 48 8b 03 8b d7 4c";
	pattern STRUCT_CTX =
		"48 8d 05 ? ? ? ? 48 8b f9 48 89 01 8b da 48 81 c1 20 02 00 00 48 8d 05";
	pattern SENDACTION =
		"e8 ? ? ? ? 8b 44 24 20 c1 e8 05";
	pattern SPAWNUI =
		"48 8b ce e8 ? ? ? ? 48 8b 4d 4f";
	//pattern NETWORK =
//		"48 89 74 24 18 57 48 83 EC 50 8B F2 49 8B F8 41 0F B7 50 02 8B CE E8 ? ? 73 FF 0F B7 57 02 8D 42 ? 3D ? ? 00 00 0F 87 60 01 00 00 4C 8D 05";
	pattern CHAT =
		"48 8B 0D ? ? ? ? BA ? ? ? ? 48 83 C1 ? E8 ? ? ? ? 83 78 ? ? 0F 85 ? ? ? ? C6 05 ? ? ? ? ?";
	pattern SRUCT_MARKET =
		"48 8d 05 ? ? ? ? 48 8b d9 48 89 01 48 81 c1 60 03";
	pattern LUA =
		"48 83 3D ? ? ? ? ? 8B 9E ? ? ? ?";
};