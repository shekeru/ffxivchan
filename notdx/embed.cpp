#include "sdk.h"
#include "gui.h"

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
	}; sys.Console(0, str); return 0;
}

static const struct luaL_Reg prints[] = {
  {"print", luaB_print},
  {NULL, NULL} /* end of array */
};

void SDK::RegisterLua() {
	auto intptr5 = (uintptr_t***)SDK::Memory->ScanPattern
		("48 83 3D ? ? ? ? ? 8B 9E ? ? ? ?", 3, 1);
	SDK::_L = (lua_State*)intptr5[0][0x583][1];
	printf("Result, %p -> %p \n", intptr5, _L);
	// Ayy lmao
	lua_getglobal(_L, "_G");
	luaL_register(_L, NULL, prints);
	lua_pop(_L, 1);
	// What the FUCK
	//_S = luaL_newstate();
	//// Further Testing
	//luaL_openlibs(_S);
	//lua_getglobal(_S, "_G");
	//luaL_register(_S, NULL, prints);
	//lua_pop(_S, 1);
	//// Init File
	LuaExec("require '_boot'");
}

void SDK::LuaFile(const char* fname) {
	static char buffer[256]; GetFullPathName
		(fname, 256, buffer, NULL);
	printf("%s\n", buffer);
	if (luaL_dofile(_L, buffer)) {
		auto line = string(lua_tostring(_L, -1));
		sys.Console(2, line); lua_pop(_L, 1);
	}
}

void SDK::LuaExec(const char* str) {
	auto new_str = new char[strlen(str) + 12]; 
	sprintf(new_str, "return %s", str);
	if (luaL_dostring(_L, new_str)) {
		auto line = string(lua_tostring(_L, -1));
		sys.Console(2, line); lua_pop(_L, 1);
	} else if(lua_gettop(_L) > 0) {
		lua_getglobal(_L, "print"); lua_insert(_L, 1);
		if (lua_pcall(_L, lua_gettop(_L) - 1, 0, 0))
			sys.Console(2, string(lua_tostring(_L, -1)));
	}; lua_settop(_L, 0); 
	delete new_str;
}