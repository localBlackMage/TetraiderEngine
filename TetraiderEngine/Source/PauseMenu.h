#pragma once
#include "Component.h"
class PauseMenu :public Component
{
public:
	PauseMenu();
	~PauseMenu();
	static Component* CreateInstance() { return new PauseMenu(); }
	void Update(float dt);
	void Serialize(const json& j);
	void HandleEvent(Event* pEvent);
	void LateInitialize();
private:
	std::vector<GameObject*> m_objects;
	std::string m_prefabName;
	bool isActive;
};

