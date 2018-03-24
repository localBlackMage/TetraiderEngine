#include <Stdafx.h>

Gate::Gate() : Component(ComponentType::C_Gate), m_isOpen(true) {}
Gate::~Gate() {}

void Gate::Update(float dt) {
	/*if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_G)) {
		OpenGate();
	}

	if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_H)) {
		CloseGate();
	}*/
}

void Gate::LateUpdate(float dt) {}

void Gate::Deactivate() {
	pGO = nullptr;
}

void Gate::Serialize(const json& j) {
	m_isHorizontal = ParseBool(j, "isHorizontal");
	m_numberOfGates = ParseInt(j, "numberOfGates");
	m_offset = ParseFloat(j, "offset");
	m_gateSpritePrefab = ParseString(j, "gateSpritePrefab");
	m_isInitilizeOnLevelInitialize = ParseBool(j, "isInitilizeOnLevelInitialize");
	m_isOpen = ParseBool(j, "isOpen");
}

void Gate::Override(const json& j) {}

void Gate::LateInitialize() {
	if (!m_isInitilizeOnLevelInitialize) {
		Init();
	}

	if(m_isOpen)
		pGO->m_isCollisionDisabled = true;
}

void Gate::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EVENT_OnLevelInitialized: {
			if (m_isInitilizeOnLevelInitialize)
				Init();
			break;
		}
		case EVENT_OpenGate: {
			OpenGate();
			break;
		}
		case EVENT_CloseGate: {
			CloseGate();
			break;
		}
	}
}

void Gate::OpenGate() {
	if (m_isOpen) return;

	for (unsigned int i = 0; i < m_pGates.size(); ++i) {
		if (m_pGates[i]) {
			Animation* pAnim = m_pGates[i]->GetComponent<Animation>(C_Animation);
			if (pAnim)
				pAnim->Play(0, true);

			TETRA_GAME_OBJECTS.SwitchGameObjectLayer(m_pGates[i], RENDER_LAYER::L_ONE);
		}
	}

	m_isOpen = true;
	pGO->m_isCollisionDisabled = true;
}

void Gate::CloseGate() {
	if (!m_isOpen) return;

	for (unsigned int i = 0; i < m_pGates.size(); ++i) {
		if (m_pGates[i]) {
			Animation* pAnim = m_pGates[i]->GetComponent<Animation>(C_Animation);
			if (pAnim)
				pAnim->Play(0, false);

			TETRA_GAME_OBJECTS.SwitchGameObjectLayer(m_pGates[i], RENDER_LAYER::L_TWO);
		}
	}

	m_isOpen = false;
	pGO->m_isCollisionDisabled = false;
}

void Gate::Init() {
	Vector3D offset;
	if (m_isHorizontal) {
		offset.Set(m_offset, 0, 0);
		m_startOffset.Set(-(float)(m_offset*m_numberOfGates)/2 + m_offset/2, 0 , 0);
	}
	else {
		offset.Set(0, m_offset, 0);
		m_startOffset.Set(0, -(float)(m_offset*m_numberOfGates) /2 + m_offset, 0);
	}

	for (int i = 0; i < m_numberOfGates; ++i) {
		m_pGates.push_back(TETRA_GAME_OBJECTS.CreateGameObject(m_gateSpritePrefab, true, pGO->GetComponent<Transform>(C_Transform)->GetPosition() + m_startOffset + offset*(float)i));
	}
}
