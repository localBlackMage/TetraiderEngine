#pragma once
#include "Component.h"
//enum class CanvasType;
class Canvas : public Component
{
private:
	std::vector<GameObject*> m_UIelements;
	bool m_isActive;
	bool m_isActiveOnAwake;
public:
	Canvas();
	~Canvas();
	static Component* CreateInstance() { return new Canvas(); }
	void Update(float);
	void AddGameObjectToCanvas(GameObject* pGO);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);
	void ActivateCanvas();
	void DeactivateCanvas();
	
	CanvasType m_canvasType;
};

