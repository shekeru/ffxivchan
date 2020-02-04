#pragma once
#include "vmt.h"
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}; 
// Lua Console
class LuaVM {
public:
	void Connect(); lua_State* L;
	void DoFile(const char* fname);
	void DoString(const char* fn_str);
	// REPL State
	struct line {
		char std; string str;
	}; deque <struct line> Items;
	void Output(char x, string ins) {
		Items.push_back({ x, ins });
	}; void Output(char x, char* ins) {
		Items.push_back({ x, ins });
	}; // Imgui Shit
}; inline LuaVM vm;