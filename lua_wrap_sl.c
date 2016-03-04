#include "sl_shader.h"

#include <lua.h>
#include <lauxlib.h>

static int
lblend(lua_State *L) {
	if (lua_isnoneornil(L,1)) {
		sl_shader_defaultblend();
	} else {
		int m1 = (int)luaL_checkinteger(L,1);
		int m2 = (int)luaL_checkinteger(L,2);
		sl_shader_blend(m1, m2);
	}
	return 0;
}

static int
lclear(lua_State *L) {
	uint32_t c = luaL_optinteger(L, 1, 0xff000000);
	sl_shader_clear(c);
	return 0;
}

static int
lversion(lua_State *L) {
	lua_pushinteger(L, sl_shader_version());
	return 1;
}

int
luaopen_sl_c(lua_State* L) {
	luaL_Reg l[] = {
		{ "blend", lblend },
		{ "clear", lclear },
		{ "version", lversion },

		{ NULL, NULL },		
	};
	luaL_newlib(L, l);
	return 1;
}