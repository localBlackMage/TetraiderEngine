#pragma once
#ifndef FLASH_ONTAKEDAMAGE_H
#define FLASH_ONTAKEDAMAGE_H

class Sprite;

#include "Component.h"
#include "Math/MathLibs.h"

class FlashOnTakeDamage : public Component {
public:
	FlashOnTakeDamage();
	~FlashOnTakeDamage();
	virtual void Update(float dt);
	virtual void Serialize(json j);
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

