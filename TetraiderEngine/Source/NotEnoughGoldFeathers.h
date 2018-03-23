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

