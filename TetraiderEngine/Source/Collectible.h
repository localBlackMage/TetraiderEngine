/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

class Collectible : public Component {
public:
	Collectible();
	~Collectible();
	static Component* CreateInstance() { return new Collectible(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
private:
	bool m_isHealthPickUp;
	bool m_isEgg;
	bool m_isAmmo;
	bool m_isGoldenFeather;
	int m_value;
};

#endif

