#pragma once
#include "Component.h"
#include "Endpoints.h"

class Slider : public Component
{
private:
	float m_currentValue;
	float m_minX;
	float m_maxX;
	float m_diff;

	//Sprite* m_pSpriteBall;
	Transform* m_pTransform;
	FollowCursor* m_pfollow;
	Endpoints* m_pEndpoints;
	bool m_isPressed;
	bool m_gotEndpoints;

public:
	Slider();
	~Slider();
	static Component* CreateInstance() { return new Slider(); }
	void Update(float);
	void Serialize(const json& j);
	void LateInitialize();
	void HandleEvent(Event* pEvent);
};

