#include "repl.h"

static int luaB_print(lua_State* L) {
	int n = lua_gettop(L); string str;
	lua_getglobal(L, "tostring");
	for (int i = 1; i <= n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
				LUA_QL("print"));
		if (i > 1) str += " "; str += s;
		lua_pop(L, 1);  /* pop result */
	}; vm.Output(0, str); return 0;
}

static const struct luaL_Reg altRegs[] = {
  {"print", luaB_print},
  {NULL, NULL} /* end of array */
};

void LuaVM::Connect() {
	IntPtr lua_pb = game->ScanPattern(Offsets::LUA, 3, 1);
	L = lua_pb[0][0x2C18][8].Cast<lua_State*>();
	printf("Lua_State: OK, %p\n", L);
	lua_getglobal(L, "_G");
	luaL_register(L, NULL, altRegs);
	lua_pop(L, 1); 
	DoFile("_boot.lua");
};

void LuaVM::DoFile(const char* fname) {
	auto ret_str = new char[strlen(fname) + 12];
	sprintf(ret_str, "../game/lua/%s", fname);
	if (luaL_dofile(L, ret_str)) {
		auto line = lua_tostring(L, -1);
		Output(2, line); lua_pop(L, 1);
	} else if (lua_gettop(L) > 0) {
		lua_getglobal(L, "print"); lua_insert(L, 1);
		if (lua_pcall(L, lua_gettop(L) - 1, 0, 0))
			Output(2, lua_tostring(L, -1));
	}; lua_settop(L, 0); delete ret_str;
}

void LuaVM::DoString(const char* fn_str) {
	auto ret_str = new char[strlen(fn_str) + 12];
	sprintf(ret_str, "return %s", fn_str);
	auto code = luaL_loadstring(L, ret_str);
	if (code) {
		lua_pop(L, 1); code =
			luaL_loadstring(L, fn_str);
	};  code = code || lua_pcall(L, 0, -1, 0);
	if (code) {
		auto line = lua_tostring(L, -1);
		Output(2, line); lua_pop(L, 1);
	}
	else if (lua_gettop(L) > 0) {
		lua_getglobal(L, "print"); lua_insert(L, 1);
		if (lua_pcall(L, lua_gettop(L) - 1, 0, 0))
			Output(2, lua_tostring(L, -1));
	}; lua_settop(L, 0); delete ret_str;
}