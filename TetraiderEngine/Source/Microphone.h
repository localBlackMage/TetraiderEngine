#pragma once
#include "Component.h"

class Transform;
class Body;
class Microphone : public Component
{
public:
	Microphone();
	~Microphone();
	static Component* CreateInstance() { return new Microphone(); }
	
	// Inherited via Component
	void Update(float dt) ;
	void Serialize(const json & j);
	void LateInitialize();
private:
	Transform* m_pObjTrans;
	Body* m_pBody;
};

