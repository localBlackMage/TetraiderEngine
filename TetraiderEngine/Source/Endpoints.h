#pragma once
#include "Component.h"
class Endpoints :public Component
{
private:
	Transform * m_pTransform;
	float m_start;
	float m_end;
	std::string m_sliderName;

public:
	Endpoints();
	~Endpoints();

	static Component* CreateInstance() { return new Endpoints(); }
	void Update(float dt);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);

	float GetMin() { return m_start; }
	float GetMax() { return m_end; }
	std::string GetSliderName() { return m_sliderName; }
};

