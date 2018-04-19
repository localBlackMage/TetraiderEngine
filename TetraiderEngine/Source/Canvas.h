/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Sujay Shah>
- End Header --------------------------------------------------------*/

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
	bool m_isAnimateOnActivation;
	bool m_isDeactivating;
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
	void DeactivateCanvas(bool isForceDeactivation = false);
	void DeactivateAfterAnimComplete();
	bool GetActiveStatus() { return m_isActive; };
	CanvasType m_canvasType;
};

