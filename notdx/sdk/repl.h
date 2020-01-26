#pragma once
#include "vmt.h"
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}; using namespace std;
#include <nlohmann/json.hpp>
using json = nlohmann::json;
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
#define local static auto
#define pattern inline const char*
// Memory Patterns
namespace Offsets {
	pattern QUEST_2 =
		"48 8d 05 ? ? ? ? 48 8d 0c db 48";
	pattern LOCAL_ACTOR =
		"48 8b 05 ? ? ? ? 48 89 6c 24 60";
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
	pattern NETWORK =
		"48 89 74 24 18 57 48 83 EC 50 8B F2 49 8B F8 41 0F B7 50 02 8B CE E8 ? ? 73 FF 0F B7 57 02 8D 42 ? 3D ? ? 00 00 0F 87 60 01 00 00 4C 8D 05";
	pattern CHAT =
		"48 8B 0D ? ? ? ? BA ? ? ? ? 48 83 C1 ? E8 ? ? ? ? 83 78 ? ? 0F 85 ? ? ? ? C6 05 ? ? ? ? ?";
	pattern SRUCT_MARKET =
		"48 8d 05 ? ? ? ? 48 8b d9 48 89 01 48 81 c1 60 03";
	pattern LUA =
		"48 83 3D ? ? ? ? ? 8B 9E ? ? ? ? ";
	pattern CAN_SWAP =
		"81 f9 2e 01 00 00 7f 39 81 f9";
	pattern GET_ICON =
		"E8 ?? ?? ?? ?? F6 DB 8B C8";
	pattern COMBO_INT64 =
		"48 89 2D ? ? ? ? 85 c0";
	pattern JOB_GAUGE =
		"48 8d 0d ? ? ? ? 0f 95 c2 e8";
	pattern FLOAT_CHECK =
		"48 8b cf e8 ? ? ? ? 84 c0 74 31";
	pattern QUEUE_ACTION =
		"40 53 55 57 41 54 41 57 48 83 ec 60";
}; 
#include "Actor.h"
class FFXIV {
public:
	Actor*& LocalActor = *game->ScanPattern(Offsets
		::LOCAL_ACTOR, 3).Cast<Actor**>();
	Combo* ComboSys = game->ScanPattern
		(Offsets::COMBO_INT64, 3).Cast<Combo*>();
	JobGauge* JobHud = game->ScanPattern
		(Offsets::JOB_GAUGE, 3).Cast<JobGauge*>();
	json tradecraft;
}; inline FFXIV* xiv;