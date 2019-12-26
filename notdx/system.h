#pragma once
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <iostream>
#include <fcntl.h>
#include <io.h>
// Extra Files
#include <string>
#include <psapi.h>
#include "d3d11.h"
#include <vector>
//Macros
#define $(value) \
	*(uint32_t*)value
#define ntxl printf("\n");
#define SINGLETON(TYPE, NAME) \
	inline TYPE* NAME;
#define INSTANCE(TYPE, NAME) \
	inline static TYPE* NAME;
using namespace std;
// Utils Area
namespace Utils {
	inline struct console {
		HANDLE err, out, in;
	} old, now; void EnableConsole(); void DisableConsole(); char ConsoleReadKey();
	void BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr);
	void ClearConsole(char fill); template<typename ptr>
		inline void PrintBytes(ptr target, int range = 16) {
		for (auto i = 0; i < range; i++) {
			printf("%hhx ", PUCHAR(target)[i]);
		}; printf("\n");
	};
}
// Extra
template< typename ptr>
inline uint32_t resolve_addr(int offset, ptr src) {
	auto jmp_start = uint32_t(src) + offset;
	return $(jmp_start) + (++jmp_start) + 4;
}

template<typename obj, typename ptr>
inline obj resolve_ops(int offset, ptr src, int width = 2) {
	return *(obj*)(uint32_t(src) + offset + width);
}

inline uint32_t* vmt_slot(void* instance, int offset) {
	return (*(uint32_t**)instance) + offset;
}

template<typename obj, typename ptr>
inline obj vmt_func(ptr src, int offset) {
	return (obj)*vmt_slot(src, offset);
}

template<typename ptr>
inline void PrintBytes(ptr target, int range = 16) {
	for (auto i = 0; i < range; i++) {
		printf("%hhx ", PUCHAR(target)[i]);
	}; printf("\n");
};
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
};
// Module Shit
class DynLib {
public:
	explicit DynLib(LPCTSTR filename) : _module(LoadLibrary(filename)) {}
	ProcPtr operator[](LPCSTR proc_name) const {
		return ProcPtr(GetProcAddress(_module, proc_name));
	}; HMODULE _module;
};