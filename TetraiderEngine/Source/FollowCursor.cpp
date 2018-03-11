//#include "FollowCursor.h"
//#include "Body.h"
//#include "Transform.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

FollowCursor::FollowCursor() :Component(ComponentType::C_FollowCursor), m_isDisableRenderOnPause(false), m_isLevelOver(false) {}
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
	if (ValueExists(j, "isDisableRenderOnPause")) {
		m_isDisableRenderOnPause = ParseBool(j, "isDisableRenderOnPause");
		if (m_isDisableRenderOnPause) {
			TETRA_EVENTS.Subscribe(EVENT_INPUT_PAUSEGAME, this);
			TETRA_EVENTS.Subscribe(EVENT_ExitLevel, this);
			TETRA_EVENTS.Subscribe(EVENT_LevelInComplete, this);
		}
	}
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
	if (m_isDisableRenderOnPause) {
		if (pEvent->Type() == EVENT_INPUT_PAUSEGAME && !m_isLevelOver) {
			InputButtonData* pData = pEvent->Data<InputButtonData>();
			if(pData->m_isTrigger)
				pGO->m_isRender = !pGO->m_isRender;
		}
		else if (pEvent->Type() == EVENT_ExitLevel) {
			pGO->m_isRender = false;
			m_isLevelOver = true;
		}
		else if (pEvent->Type() == EVENT_LevelInComplete) {
			pGO->m_isRender = false;
			m_isLevelOver = true;
		}
	}
}
