/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef LEVEL_STATUS_H
#define LEVEL_STATUS_H

class LevelStatus : public Component {
public:
	LevelStatus();
	~LevelStatus();
	static Component* CreateInstance() { return new LevelStatus(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
private:
	int m_eggsToCollect;
	int m_eggsCollected;
	int m_enemiesInLevel;
};

#endif

