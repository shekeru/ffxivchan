#pragma once
#define IUnknown__QueryInterface 0
#define IUnknown__AddRef 1
#define IUnknown__Release 2
#define IDXGIObject__SetPrivateData  3
#define IDXGIObject__SetPrivateDataInterface 4
#define IDXGIObject__GetPrivateData 5
#define IDXGIObject__GetParent 6
#define IDXGIFactory__EnumAdapters 7
#define IDXGIFactory__MakeWindowAssociation 8
#define IDXGIFactory__GetWindowAssociation 9
#define IDXGIFactory__CreateSwapChain 10
#include "memory.h"
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
// SDK Namespace
namespace SDK {
	inline lua_State *_L, *_S;
	SINGLETON(MemorySystem, Memory);
	void LuaExec(const char* fn_str, bool show = 1);
	void RegisterLua();
};
// Hooks Namespace
namespace Hooks {
	HRESULT _fastcall Present(IDXGISwapChain *pChain, UINT SyncInterval, UINT Flags);
	HRESULT _fastcall CreateSwapChain(IDXGIFactory* pFactory, ID3D11Device *pDevice,
		DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain);
};