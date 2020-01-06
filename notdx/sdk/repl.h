#pragma once
#include "vmt.h"
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}; using namespace std;
// SDK Namespace
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
		Items.push_back({ x, ins});
	}; // Imgui Shit
}; inline LuaVM vm;
// Memory Patterns
#define pattern inline const char*
namespace Offsets {
	pattern SENDACTION =
		"e8 ? ? ? ? 8b 44 24 20 c1 e8 05";
	pattern SPAWNUI =
		"48 8b ce e8 ? ? ? ? 48 8b 4d 4f";
	pattern NETWORK =
		"48 89 74 24 18 57 48 83 EC 50 8B F2 49 8B F8 41 0F B7 50 02 8B CE E8 ? ? 73 FF 0F B7 57 02 8D 42 ? 3D ? ? 00 00 0F 87 60 01 00 00 4C 8D 05";
	pattern CHAT =
		"48 8B 0D ? ? ? ? BA ? ? ? ? 48 83 C1 ? E8 ? ? ? ? 83 78 ? ? 0F 85 ? ? ? ? C6 05 ? ? ? ? ?";
	pattern MARKET =
		"48 8d 05 ? ? ? ? 48 8b d9 48 89 01 48 81 c1 60 03";
	pattern LUA =
		"48 83 3D ? ? ? ? ? 8B 9E ? ? ? ? ";
};