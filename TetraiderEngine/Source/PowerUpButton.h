#pragma once
#ifndef POWERUP_BUTTON_H
#define POWERUP_BUTTON_H

class PowerUpButton :public Component
{
private:
	Sprite* m_pSprite;
	Text* m_pPowerUpText;
	PowerUp m_powerUp;
	PowerUpCategory m_category;
	std::string m_powerUpIconPrefab;
	int m_levelNumber;
public:
	PowerUpButton();
	~PowerUpButton();
	static Component* CreateInstance() { return new PowerUpButton(); }
	void Update(float dt);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);
};

#endif

