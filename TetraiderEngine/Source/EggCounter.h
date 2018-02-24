#pragma once
#ifndef EGG_COUNTER_H
#define EGG_COUNTER_H

#include "Component.h"
#include "Math/MathLibs.h"

class Text;

class EggCounter : public Component {
public:
	EggCounter();
	~EggCounter();
	static Component* CreateInstance() { return new EggCounter(); }
	virtual void DeActivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
private:
	Text* m_pText;
	int m_eggsToCollect;
	int m_eggsCollected;
};

#endif

