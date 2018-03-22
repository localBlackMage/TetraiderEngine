#pragma once
#include "Component.h"
//enum class CanvasType;
class Canvas : public Component
{
private:
	std::vector<GameObject*> m_UIelements;
	bool m_isActive;
	bool m_isActiveOnAwake;
	Text* m_pText;
	bool m_isLevelOver;
	bool m_isCannotPause;

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
	bool GetActiveStatus() { return m_isActive; };
	CanvasType m_canvasType;
};

