#pragma once
#include "Component.h"
#include "GameObject.h"
class CutScene :public Component
{
private:
	GameObject* m_Scenes[13];
public:
	CutScene();
	~CutScene();
	static Component* CreateInstance() { return new CutScene(); }
	// Inherited via Component
	 void Update(float dt);
	 void Serialize(const json & j);
	 void HandleEvent(Event* pEvent);
	 
};



