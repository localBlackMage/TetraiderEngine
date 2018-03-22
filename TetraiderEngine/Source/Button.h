#pragma once

#include "CanvasTypeName.h"

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
	CanvasType m_canvasToActivate;
	CanvasType m_canvasToDeActivate;
	//Text* m_pText;
public:
	Button();
	~Button();
	static Component* CreateInstance() { return new Button(); }
	void Update(float dt);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);
};

