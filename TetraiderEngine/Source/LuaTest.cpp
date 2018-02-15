#include "LuaTest.h"
#include "Transform.h"
#include "Body.h"

#include <iostream>

LuaTest::LuaTest() :
	Component(ComponentType::LuaTest)
{
}

LuaTest::~LuaTest() {}

void LuaTest::Update(float dt) {
	// Create lua state
	lua_State* L = luaL_newstate();
	if (L == NULL) { // error checking
		std::cout << "luaL_newstate() failed\n";
	}
	// TODO: 
	// Push c++ function to lua
	// getGlobalNamespace(L).addFunction("LuaPrint", LuaPrint);
	// Opening lua stuff
	luaL_dofile(L, "LuaScript/script.lua");
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);
	// fetch some data from the script
	LuaRef s = getGlobal(L, "isEnabled");
	LuaRef px = getGlobal(L, "LuaResetX");
	LuaRef py = getGlobal(L, "LuaResetY");
	// re-cast to c++ datatype
	bool isLuaEnabled = s.cast<bool>();
	float PosX = px.cast<float>();
	float PosY = py.cast<float>();

	// testing reset position
	if (isLuaEnabled) {
		LuaReset(PosX, PosY);
	}
	// fetch lua function
	//LuaRef testFn = getGlobal(L, "testFn");
	//int resShouldBeZero = testFn();
	//std::cout << "Result of testFn(): " << resShouldBeZero << "\n";
}

void LuaTest::Serialize(json j) {
}

void LuaTest::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentType::Transform));
		else {
			printf("No Game Object found. Controller component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Controller component failed to operate.\n");
			return;
		}
	}
}

void LuaTest::LuaReset(float x, float y) {
	m_pTransform->SetPosition(Vector3D(x, y, 0.0f));
}

void LuaTest::LuaPrint(const std::string& str) {
	std::cout << str << "\n";
}