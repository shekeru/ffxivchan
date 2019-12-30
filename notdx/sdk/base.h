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
	inline const char* CHAT = "48 8B 0D ? ? ? ? BA ? ? ? ? 48 83 C1 ? E8 ? ? ? ? 83 78 ? ? 0F 85 ? ? ? ? C6 05 ? ? ? ? ?";
	inline const char* MARKET = "48 8d 05 ? ? ? ? 48 8b d9 48 89 01 48 81 c1 60 03";
	inline const char* LUA = "48 83 3D ? ? ? ? ? 8B 9E ? ? ? ? ";
};
#include <set>
inline std::set <uintptr_t> objects;