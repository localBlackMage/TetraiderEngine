#include <Stdafx.h>

EggCounter::EggCounter(): Component(ComponentType::C_EggCounter), m_eggsCollected(0), m_eggsToCollect(3) {}
EggCounter::~EggCounter() {}

void EggCounter::DeActivate() {
	pGO = nullptr;
}

void EggCounter::Update(float dt) {}

void EggCounter::Serialize(const json& j) {}

void EggCounter::LateInitialize() {
	if (!m_pText) {
		if (pGO)
			m_pText = pGO->GetComponent<Text>(ComponentType::C_Text);
		else {
			printf("No Game Object found. EggCounter component failed to operate.\n");
			return;
		}

		if (!m_pText) {
			printf("No Text component found. EggCounter component failed to operate.\n");
			assert(m_pText);
			return;
		}
	}
	
	TETRA_EVENTS.Subscribe(EVENT_EggCounterInitialization, this);
	TETRA_EVENTS.Subscribe(EVENT_EggCollected, this);
	TETRA_EVENTS.Subscribe(EVENT_LevelComplete, this);
}

void EggCounter::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EventType::EVENT_EggCounterInitialization: {
			CollectibleData* pData = pEvent->Data<CollectibleData>();
			m_eggsToCollect = pData->m_value;
			m_eggsCollected = 0;
			m_pText->SetText(std::to_string(m_eggsCollected) + "/" + std::to_string(m_eggsToCollect));
			break;
		}
		case EventType::EVENT_EggCollected: {
			CollectibleData* pData = pEvent->Data<CollectibleData>();
			m_eggsCollected += pData->m_value;
			m_pText->SetText(std::to_string(m_eggsCollected) + "/" + std::to_string(m_eggsToCollect));
			break;
		}
		case EventType::EVENT_LevelComplete: {
			pGO->m_isRender = false;
		}
	}
}