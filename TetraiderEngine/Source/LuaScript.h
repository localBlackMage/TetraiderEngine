/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: LuaScript.h
Purpose: Handles Lua-scripting. Singleton class.
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 1/27/18
- End Header --------------------------------------------------------*/

#ifndef LUASCRIPT_H
#define LUASCRIPT_H

extern "C" {
#include "External\Lua\lua.hpp"
}
#include "External\Lua\LuaBridge\LuaBridge.h"
#include <string>

class LuaScript
{
private:	
	lua_State* m_LuaState;

	LuaScript();
	~LuaScript();
public:
	LuaScript(const LuaScript &) = delete;
	void operator=(const LuaScript &) = delete;

	static LuaScript& GetInstance()
	{
		static LuaScript instance;
		return instance;
	}
	void LuaInit(const char* luaFile);
	lua_State* GetLuaState();

};

#endif