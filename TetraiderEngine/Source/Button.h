#pragma once
#include "Component.h"
#include "Text.h"
class Sprite;

class Button :public Component
{
private:
	std::string Text;
	Sprite* m_pSprite;
	int m_levelNumber;
	bool isQuit;
	bool isRestart;
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

