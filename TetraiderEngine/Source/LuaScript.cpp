#include "LuaScript.h"

#include <iostream>

using namespace luabridge;

LuaScript::LuaScript(){
	m_LuaState = luaL_newstate();
	if (m_LuaState== NULL){
		std::cout<< "luaL_newstate() failed\n";
	}
}

LuaScript::~LuaScript() {
	lua_close(m_LuaState);
}

void LuaScript::LuaInit(const char* luaFile){
	luaL_dofile(m_LuaState, luaFile);
	luaL_openlibs(m_LuaState);
	lua_pcall(m_LuaState, 0, 0, 0);
}

lua_State* LuaScript::GetLuaState(){
	return m_LuaState;
}