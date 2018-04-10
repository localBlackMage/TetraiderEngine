#include <Stdafx.h>

GateTriggerBox::GateTriggerBox(): Component(ComponentType::C_GateTriggerBox), m_isTriggered(false), m_isBossTriggerBox(false) {}
GateTriggerBox::~GateTriggerBox() {}

void GateTriggerBox::Update(float dt) {}

void GateTriggerBox::LateUpdate(float dt) {
	for (unsigned int i = 0; i < m_gatePositions.size(); ++i) {
		TETRA_DEBUG.DrawLine(pGO->GetComponent<Transform>(C_Transform)->GetPosition(), pGO->GetComponent<Transform>(C_Transform)->GetPosition() + m_gatePositions[i].m_pGatePos, DebugColor::CYAN);
	}
}

void GateTriggerBox::Deactivate() {
	pGO = nullptr;
}

void GateTriggerBox::Serialize(const json& j) {
	int gatesToSpawn = j["gatePositions"].size();
	m_gatePrefabHorizontal = ParseString(j, "gatePrefabHorizontal");
	m_gatePrefabVertical = ParseString(j, "gatePrefabVertical");
	m_isBossTriggerBox = ParseBool(j, "isBossTriggerBox");
	for (int i = 0; i < gatesToSpawn; ++i) {
		m_gatePositions.push_back(GateInfo(ParseVector3D(j["gatePositions"][i], "position"), ParseBool(j["gatePositions"][i], "isHorizontal")));
	}

	if (m_isBossTriggerBox) {
		TETRA_EVENTS.Subscribe(EVENT_OnBossDefeated, this);
	}
}

void GateTriggerBox::Override(const json& j) {
	if (ValueExists(j, "gatePositions")) {
		m_gatePositions.clear();
		int gatesToSpawn = j["gatePositions"].size();
		for (int i = 0; i < gatesToSpawn; ++i) {
			m_gatePositions.push_back(GateInfo(ParseVector3D(j["gatePositions"][i], "position"), ParseBool(j["gatePositions"][i], "isHorizontal")));
		}
	}
}

void GateTriggerBox::LateInitialize() {}

void GateTriggerBox::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EVENT_OnLevelInitialized: {
			for (unsigned int i = 0; i < m_gatePositions.size(); ++i) {
				if(m_gatePositions[i].m_isHorizontal)
					m_gates.push_back(TETRA_GAME_OBJECTS.CreateGameObject(m_gatePrefabHorizontal, true, pGO->GetComponent<Transform>(C_Transform)->GetPosition() + m_gatePositions[i].m_pGatePos));
				else
					m_gates.push_back(TETRA_GAME_OBJECTS.CreateGameObject(m_gatePrefabVertical, true, pGO->GetComponent<Transform>(C_Transform)->GetPosition() + m_gatePositions[i].m_pGatePos));
			}
			break;
		}
		case EVENT_OnCollide: {
			if (m_isTriggered) return;
			OnCollideData* pData = pEvent->Data<OnCollideData>();

			if (pData->pGO->m_tag == T_Player) {
				m_isTriggered = true;
				pGO->m_isCollisionDisabled = true;
				Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
				if (pAudio)
					pAudio->Play(0);
				for (unsigned int i = 0; i < m_gates.size(); ++i) {
					if (m_gates[i]) {
						m_gates[i]->HandleEvent(&Event(EventType::EVENT_CloseGate));
					}
				}

				if (m_isBossTriggerBox) {
					TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_OnEnterBoss));
				}
			}
			break;
		}
		case EVENT_OnBossDefeated: {
			if (m_isBossTriggerBox) {
				Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
				if (pAudio)
					pAudio->Play(1);
				for (unsigned int i = 0; i < m_gates.size(); ++i) {
					if (m_gates[i]) {
						m_gates[i]->HandleEvent(&Event(EventType::EVENT_OpenGate));
					}
				}
			}
		}
	}
}

Vector3D GateTriggerBox::GetGatePos(int index) {
	if(index >= (int)m_gatePositions.size())
		return Vector3D();

	return m_gatePositions[index].m_pGatePos;
}

void GateTriggerBox::SetGatePos(int index, const Vector3D& pos) {
	if (index >= (int)m_gatePositions.size())
		return;

	m_gatePositions[index].m_pGatePos = pos;
}

void GateTriggerBox::SetGatePosSize(int size) {
	if ((int)m_gatePositions.size() == size)
		return;
	else if ((int)m_gatePositions.size() < size) {
		int start = (int)m_gatePositions.size();
		for (int i = start; i < size; ++i) {
			m_gatePositions.push_back(GateInfo(Vector3D(), false));
		}
	}
	else if ((int)m_gatePositions.size() > size) {
		int start = (int)m_gatePositions.size();
		for (int i = start; i > size; --i) {
			m_gatePositions.pop_back();
		}
	}
}

int GateTriggerBox::GetNumberOfGates() {
	return (int)m_gatePositions.size();
}

bool GateTriggerBox::IsHorizontal(int index) {
	if (index >= (int)m_gatePositions.size())
		return false;

	return m_gatePositions[index].m_isHorizontal;
}

void GateTriggerBox::SetAlignment(int index, bool isHorizontal) {
	if (index >= (int)m_gatePositions.size())
		return;

	m_gatePositions[index].m_isHorizontal = isHorizontal;
}
