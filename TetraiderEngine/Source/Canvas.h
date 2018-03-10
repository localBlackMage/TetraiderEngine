#pragma once
#include "Component.h"
class Canvas : public Component
{
private:
	std::vector<GameObject*> m_UIelements;
	bool m_isActive;
	int m_canvasType;
	bool m_isActiveOnAwake;
public:
	Canvas();
	~Canvas();
	static Component* CreateInstance() { return new Canvas(); }
	void Update(float);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);
	void ActivateCanvas();
	void DeactivateCanvas();

	
	
};

