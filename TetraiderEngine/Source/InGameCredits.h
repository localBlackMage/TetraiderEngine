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

