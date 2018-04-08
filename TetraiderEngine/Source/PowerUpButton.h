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
	GameObject* m_pPrice;
	Vector3D m_offsetForPrice;
	std::string m_powerUpIconPrefab;
	std::string m_powerUpPrice;
	int m_specialPowerIndex;
	int m_levelNumber;
	bool m_isSpecial;
	bool m_isHover;
public:
	PowerUpButton();
	~PowerUpButton();
	static Component* CreateInstance() { return new PowerUpButton(); }
	void Update(float dt);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);
	void LateUpdate(float dt);
};

#endif

