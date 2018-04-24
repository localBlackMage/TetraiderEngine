/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Sujay Shah>
- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
class ListSelector :
	public Component
{
private:
	float m_spawnOffset;
	float m_textOffsetX;
	float m_textOffsetY;
	Text* m_pText;
	Transform* m_pListTransform;
	
	bool m_isResolution;
	std::string m_leftSelectorPrefab;
	std::string m_rightSelecetorPrefab;
	
public:
	std::vector<std::string>m_Choice;
	int m_defaultChoice;
	int m_currentChoice;

	ListSelector();
	~ListSelector();
	static Component* CreateInstance() { return new ListSelector(); }

	int GetisResolution() { return m_isResolution; };
	void Update(float dt);
	void Serialize(const json & j);
	void HandleEvent(Event* pEvent);
	void LateInitialize();
};

