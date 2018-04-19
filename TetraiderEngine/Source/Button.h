/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Sujay Shah>
- End Header --------------------------------------------------------*/

#pragma once
#include "CanvasTypeName.h"

enum class ChoiceType
{
	CHOICE_RESOLUTION,
	CHOICE_FULLSCREEN,

	CHOICE_NUM
};

class Button :public Component
{
private:
	std::string Text;
	Sprite* m_pSprite;
	int m_levelNumber;
	bool m_isLoadLevelEditor;
	bool m_isQuit;
	bool m_isRestart;
	bool m_isRandomGenerated;
	bool m_isLoadCanvas;
	bool m_isCloseShop;
	bool m_isResume;
	bool m_isHover;
	bool m_isActive;
	bool m_isApplyButton;
	CanvasType m_canvasToActivate;
	CanvasType m_canvasToDeActivate;
	int m_Choice[2];
	//Text* m_pText;
public:
	Button();
	~Button();
	static Component* CreateInstance() { return new Button(); }
	void Update(float dt);
	void LateUpdate(float dt);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);
};

