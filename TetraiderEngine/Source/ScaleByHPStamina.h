/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef SCALE_BY_HP_STAMINA_H
#define SCALE_BY_HP_STAMINA_H

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
	bool m_isBoss;
	Transform* m_pTransform;
};

#endif

