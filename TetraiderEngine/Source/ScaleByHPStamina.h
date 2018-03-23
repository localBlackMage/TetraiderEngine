#pragma once
#ifndef SCALE_BY_HP_STAMINA_H
#define SCALE_BY_HP_STAMINA_H

//class Sprite;
//class Transform;
//
//#include "Component.h"
//#include "Math/MathLibs.h"

class ScaleByHPStamina : public Component {
public:
	ScaleByHPStamina();
	~ScaleByHPStamina();
	static Component* CreateInstance() { return new ScaleByHPStamina(); }
	virtual void DeActivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
	void SetOriginalScale(float scale, bool isApplyToTransform = true);
private:
	bool m_isScaleByHealth;
	float m_originalScale;
	bool m_isPlayer;
	Transform* m_pTransform;
};

#endif

