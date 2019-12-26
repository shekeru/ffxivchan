#include "sdk.h"
#include "gui.h"

static int force_print(lua_State* L) {
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
	}; sys.Console(0, str); return 0;
}

static const struct luaL_Reg prints[] = {
  {"print", force_print},
  {NULL, NULL} /* end of array */
};

void SDK::RegisterLua() {
	auto intptr5 = (uintptr_t***)SDK::Memory->ScanPattern
		("48 83 3D ? ? ? ? ? 8B 9E ? ? ? ?", 3, 1);
	SDK::_L = (lua_State*)intptr5[0][0x583][1];
	printf("Result, %p -> %p \n", intptr5, _L);
	lua_getglobal(_L, "_G");
	luaL_register(_L, NULL, prints);
	lua_pop(_L, 1);
}

void SDK::LuaExec(const char* str) {
	if (luaL_dostring(_L, str)) sys.Console
	(2, string(lua_tostring(_L, -1)));
};