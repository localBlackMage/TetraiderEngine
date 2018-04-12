#include <Stdafx.h>

FollowCursor::FollowCursor() :Component(ComponentType::C_FollowCursor), m_isDisableRenderOnPause(false), m_isLevelOver(false), m_isSliderBallSeleced(false) {}
FollowCursor::~FollowCursor() {}


void FollowCursor::Update(float dt)
{
	m_mousePos.x = (float)TETRA_INPUT.MousePosX() - (float)(TETRA_RENDERER.WindowWidth() / 2);
	m_mousePos.y = -(float)TETRA_INPUT.MousePosY() + (float)(TETRA_RENDERER.WindowHeight() / 2);
	m_pTransform->SetPosition(m_mousePos);
}

void FollowCursor::Serialize(const json & j)
{
	if (ValueExists(j, "isDisableRenderOnPause")) {
		m_isDisableRenderOnPause = ParseBool(j, "isDisableRenderOnPause");
		if (m_isDisableRenderOnPause) {
			TETRA_EVENTS.Subscribe(EVENT_OnPauseGame, this);
			TETRA_EVENTS.Subscribe(EVENT_OnGameResume, this);
			TETRA_EVENTS.Subscribe(EVENT_ExitLevel, this);
			TETRA_EVENTS.Subscribe(EVENT_LevelInComplete, this);
			TETRA_EVENTS.Subscribe(EVENT_ShopOpened, this);
			TETRA_EVENTS.Subscribe(EVENT_ShopClosed, this);
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
	if (m_isDisableRenderOnPause)
	{
		if (pEvent->Type() == EVENT_OnGameResume && !m_isLevelOver)
		{
			pGO->m_isRender = true;
		}
		else if (pEvent->Type() == EVENT_OnPauseGame && !m_isLevelOver)
		{
			pGO->m_isRender = false;
		}
		else if (pEvent->Type() == EVENT_ShopOpened)
		{
			pGO->m_isRender = false;
		}
		else if (pEvent->Type() == EVENT_ShopClosed)
		{
			pGO->m_isRender = true;
		}
		else if (pEvent->Type() == EVENT_ExitLevel)
		{
			pGO->m_isRender = false;
			m_isLevelOver = true;
		}
		else if (pEvent->Type() == EVENT_LevelInComplete)
		{
			pGO->m_isRender = false;
			m_isLevelOver = true;
		}
	}
}
