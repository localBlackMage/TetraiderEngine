#pragma once
#include "Component.h"

class Transform;
class Body;
class FollowCursor : public Component
{
private:
	Transform * m_pTransform;
	bool m_isDisableRenderOnPause;
	bool m_isLevelOver;
public:
	FollowCursor();
	~FollowCursor();
	static Component* CreateInstance() { return new FollowCursor(); }
	// Inherited via Component
    void Update(float dt);
	void Serialize(const json & j);
	void LateInitialize();
	//----end pure virtual fn

	void HandleEvent(Event* pEvent);
};

