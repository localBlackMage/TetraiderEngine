#pragma once
#include "Component.h"

class Text;

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
	Text* m_pText;
	bool isActive;
	bool isLevelOver;
};
