/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

class SplashScreen :public Component
{
public:
	SplashScreen();
	~SplashScreen();
	static Component* CreateInstance() { return new SplashScreen(); }
	void Update(float dt);
	void Serialize(const json& j);
	void HandleEvent(Event* pEvent);
	void LateInitialize();
private:
	int m_levelToLoad;
};

