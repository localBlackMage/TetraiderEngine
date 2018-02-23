#include "FollowCursor.h"
#include "Body.h"
#include "Transform.h"
#include "TetraiderAPI.h"
FollowCursor::FollowCursor() :Component(ComponentType::C_FollowCursor) {}
FollowCursor::~FollowCursor() {}

Vector3D mouse;
void FollowCursor::Update(float dt)
{
	mouse.x = (float)TETRA_INPUT.MousePosX() - (float)(TETRA_RENDERER.WindowWidth() / 2);
	mouse.y = -(float)TETRA_INPUT.MousePosY() + (float)(TETRA_RENDERER.WindowHeight() / 2);
	m_pTransform->SetPosition(mouse);
	
}

void FollowCursor::Serialize(const json & j)
{
}

void FollowCursor::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. Follow cursor component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Follow cursor component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}
}

void FollowCursor::HandleEvent(Event * pEvent)
{	
}
