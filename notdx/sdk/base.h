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
#include <set>
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
	inline static TYPE* NAME
#define ORIGINAL(type, location) \
	static auto original = decltype(&type)(location)
#define BINDING(type) static type _fastcall
// Local Macros
using namespace std;
#define local static auto
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
}; // Hooks Namespace
namespace Hooks {
	HRESULT _fastcall Present(IDXGISwapChain *pChain, UINT SyncInterval, UINT Flags);
	HRESULT _fastcall CreateSwapChain(IDXGIFactory* pFactory, ID3D11Device *pDevice,
		DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain);
	void RaptureAttach(), MarketAttach(), NetworkAttach(), ContextAttach();
}; 