#pragma once
#include "Component.h"
class Endpoints :public Component
{
private:
	Transform * m_pTransform;
public:
	Endpoints();
	~Endpoints();

	static Component* CreateInstance() { return new Endpoints(); }
	void Update(float dt);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);

	float m_start;
	float m_end;
};

