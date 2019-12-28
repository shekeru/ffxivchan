#pragma once
#define NOMINMAX
#include <cstdint>
#include <stdexcept>
#include <iostream>
// Windows Header Files
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <fcntl.h>
#include <io.h>
// Data Types
#include <deque>
#include <string>
#include <vector>
#include <map>
// Extra Stuff
#include "d3d11.h"
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
#define $(value) \
	*(uint32_t*)value
#define ntxl printf("\n");
#define SINGLETON(TYPE, NAME) \
	inline TYPE* NAME;
#define INSTANCE(TYPE, NAME) \
	inline static TYPE* NAME;
// Utils Area
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
};