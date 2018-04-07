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

