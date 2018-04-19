/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef INGAMECREDITS_H
#define INGAMECREDITS_H

class InGameCredits :public Component
{
private:

public:
	InGameCredits();
	~InGameCredits();

	static Component* CreateInstance() { return new InGameCredits(); }
	void Update(float dt);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);
};

#endif

