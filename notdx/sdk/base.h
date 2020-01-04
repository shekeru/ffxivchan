#pragma once
// Windows Header Files
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "d3d11.h"
// Data Types
#include <deque>
#include <string>
#include <vector>
#include <map>
// Library Shit
class ProcPtr {
public:
	explicit ProcPtr(FARPROC ptr) : _ptr(ptr) {}
	template <typename T, typename = std::enable_if_t<std::is_function_v<T>>>
	operator T *() const {
		return reinterpret_cast<T *>(_ptr);
	}
private:
	FARPROC _ptr;
}; // Module Shit
class DynLib {
public:
	explicit DynLib(LPCTSTR filename) : _module(LoadLibrary(filename)) {}
	ProcPtr operator[](LPCSTR proc_name) const {
		return ProcPtr(GetProcAddress(_module, proc_name));
	}; HMODULE _module;
}; // Macros
#define INSTANCE(TYPE, NAME) \
	inline static TYPE* NAME;
// Pointer Shit
class IntPtr {
public:
	IntPtr(uintptr_t value) {
		this->value = value;
	}; uintptr_t value;
	IntPtr operator[](uintptr_t next) const {
		return IntPtr(*(uintptr_t*)(value + next));
	}; template<typename obj> 
	obj Cast() {
		return (obj) value;
	};
}; // Utils Area
namespace Utils {
	inline struct console {
		HANDLE err, out, in;
	} old, now; void EnableConsole(); void DisableConsole(); char ConsoleReadKey();
	void BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr);
	void ClearConsole(char fill);  template<typename ptr>
		inline void PrintBytes(ptr target, int range = 16) {
		for (auto i = 0; i < range; i++) {
			printf("%hhx ", PUCHAR(target)[i]);
		}; printf("\n");
	};
}; // Memory Patterns
namespace Offsets {
	inline const char* SENDACTION =
		"e8 ? ? ? ? 8b 44 24 20 c1 e8 05";
	inline const char* SPAWNUI = 
		"48 8b ce e8 ? ? ? ? 48 8b 4d 4f";
	inline const char* NETWORK = 
		"48 89 74 24 18 57 48 83 EC 50 8B F2 49 8B F8 41 0F B7 50 02 8B CE E8 ? ? 73 FF 0F B7 57 02 8D 42 ? 3D ? ? 00 00 0F 87 60 01 00 00 4C 8D 05";
	inline const char* CHAT = 
		"48 8B 0D ? ? ? ? BA ? ? ? ? 48 83 C1 ? E8 ? ? ? ? 83 78 ? ? 0F 85 ? ? ? ? C6 05 ? ? ? ? ?";
	inline const char* MARKET =
		"48 8d 05 ? ? ? ? 48 8b d9 48 89 01 48 81 c1 60 03";
	inline const char* LUA = 
		"48 83 3D ? ? ? ? ? 8B 9E ? ? ? ? ";
};
#include <set>
inline std::set <uintptr_t> objects;