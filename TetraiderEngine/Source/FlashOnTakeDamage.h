/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef FLASH_ONTAKEDAMAGE_H
#define FLASH_ONTAKEDAMAGE_H

class FlashOnTakeDamage : public Component {
public:
	FlashOnTakeDamage();
	~FlashOnTakeDamage();
	static Component* CreateInstance() { return new FlashOnTakeDamage(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
private:
	float m_decaySpeed;
	float m_flashTime;
	Vector3D m_flashColor;
	Vector3D m_originalColor;
	Sprite* m_pSprite;
	void Flash();
};

#endif

