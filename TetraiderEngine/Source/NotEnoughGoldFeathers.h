/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

class NotEnoughGoldFeathers :public Component
{
public:
	NotEnoughGoldFeathers();
	~NotEnoughGoldFeathers();
	static Component* CreateInstance() { return new NotEnoughGoldFeathers(); }
	void Update(float dt);
	void Serialize(const json& j);
	void HandleEvent(Event* pEvent);
	void LateInitialize();
private:
	
};

