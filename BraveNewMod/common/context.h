#pragma once
#include "../overlay/im_engine.h"
#include "memory.h"
#include "vmt.h"

// Funny Macros
#define local \
	static auto
#define foreign \
	static auto&
#define get_context \
	static Context& ctx = Context::Current()
#define $vmt ctx._vmt

class Context {
public:
	bool Stop = false;
	inline static MemorySystem game = MemorySystem(L"ffxiv_dx11.exe");
	IM_Engine gui;

	Context() {
		printf("Context Created!\n");
	};

	~Context() {
	}

	static Context& Current(bool main_thread = false)
	{
		static std::unique_ptr<Context> _ctx = 0;
		if (!main_thread)
			while (!_ctx)
				::Sleep(750);
		else
			_ctx = std::make_unique<Context>();
		return *_ctx;
	}; 

	// Virtual Tables
	struct vmts {
		VMT* vDXGI_Factory, *vSwapChain;
	} _vmt;
};

// Misc Utils
namespace Utils {
	void EnableConsole();
	void DisableConsole();
	void BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr);
};