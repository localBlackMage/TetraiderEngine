#include <Stdafx.h>

Shop::Shop() :Component(ComponentType::C_Shop), m_isShopOpen(false), m_isListenToInput(false)
{
	TETRA_EVENTS.Subscribe(EVENT_INPUT_OPENSHOP, this);
	TETRA_EVENTS.Subscribe(EVENT_ShopClosed, this);
}

Shop::~Shop() {}

void Shop::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	if (!m_isShopOpen) {
		const GameObject* pPlayer = TETRA_GAME_OBJECTS.GetPlayer();
		if (StaticPointToStaticCircle(pPlayer->GetComponent<Transform>(C_Transform)->GetPosition(), pGO->GetComponent<Transform>(C_Transform)->GetPosition(), m_radius))
			m_isListenToInput = true;
		else
			m_isListenToInput = false;

		m_pControlsObject->m_isRender = m_isListenToInput;
	}
	else
		m_pControlsObject->m_isRender = false;

	TETRA_DEBUG.DrawWireCircle(pGO->GetComponent<Transform>(C_Transform)->GetPosition(), m_radius*2.0f, DebugColor::CYAN);
}

void Shop::Serialize(const json & j) {
	m_radius = ParseFloat(j, "radius");
	m_controlsPrefab = ParseString(j, "controlsPrefab");
	m_posOffset = ParseVector3D(j, "offset");
}

void Shop::LateInitialize() {
}

void Shop::HandleEvent(Event* pEvent)
{
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		m_pControlsObject = TETRA_GAME_OBJECTS.CreateGameObject(m_controlsPrefab, true, pGO->GetComponent<Transform>(C_Transform)->GetPosition() + m_posOffset);
		m_pControlsObject->m_isRender = false;
	}

	if (!m_isShopOpen && m_isListenToInput) {
		if (pEvent->Type() == EventType::EVENT_INPUT_OPENSHOP) {
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isTrigger) {
				m_isShopOpen = true;
				TETRA_UI.ActivateCanvas(CanvasType::CANVAS_SHOP);
				TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_ShopOpened));
			}
		}
	}
	else if (m_isShopOpen) {
		if (pEvent->Type() == EventType::EVENT_ShopClosed) {
			m_isShopOpen = false;
			TETRA_UI.DeactivateCanvas(CanvasType::CANVAS_SHOP);
		}
	}
}

