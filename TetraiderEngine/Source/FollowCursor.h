#pragma once
#include "Component.h"

class Transform;
class Body;
class FollowCursor : public Component
{
private:
	Transform * m_pTransform;
	Body* m_pBody;
public:
	FollowCursor();
	~FollowCursor();

	// Inherited via Component
    void Update(float dt);
	void Serialize(const json & j);
	//----end pure virtual fn

	void HandleEvent(Event* pEvent);
};

