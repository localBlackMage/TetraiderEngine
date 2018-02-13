#ifndef LUATEST_H
#define LUATEST_H

#include "Component.h"
extern "C" {
#include "External\Lua\lua.hpp"
}
#include "External\Lua\LuaBridge\LuaBridge.h"

using namespace luabridge;

// Forward declaration
class Transform;
class Body;

class LuaTest : 
	public Component {
private:
	Transform* m_pTransform;

public:
	LuaTest();
	~LuaTest();

	virtual void Update(float dt);
	virtual void LateInitialize();
	virtual void Serialize(json j);
	virtual void Override(json j) {};
	void LuaReset(float x, float y);
	void LuaTest::LuaPrint(const std::string& str);
};

#endif

