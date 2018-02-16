#include "FollowCursor.h"
#include "Body.h"
#include "Transform.h"
#include "TetraiderAPI.h"
FollowCursor::FollowCursor() :Component(ComponentType::C_FollowCursor) {}
FollowCursor::~FollowCursor() {}

Vector3D mouse;
void FollowCursor::Update(float dt)
{
	//TODO
	//check for collisions
	//fire event if clicked
	/*if (TETRA_INPUT.IsMouseButtonPressed)
	{
		TETRA_EVENTS.BroadcastEvent(&Event(EventType::EVENT_MOUSEBTN_PRESSED));
	}*/
	mouse.x = (float)TETRA_INPUT.MousePosRelX();
	mouse.y = (float)TETRA_INPUT.MousePosRelY();
	m_pTransform->SetPosition(mouse);
}

void FollowCursor::Serialize(const json & j)
{
}

void FollowCursor::HandleEvent(Event * pEvent)
{
	/*if ()
	{

	}*/
	//if (pEvent->Type == EventType::EVENT_MOUSEBTN_PRESSED)
	//{
	//	//load next game state
	//}
}
