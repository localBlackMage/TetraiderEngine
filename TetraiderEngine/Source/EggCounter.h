/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef EGG_COUNTER_H
#define EGG_COUNTER_H

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

