/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

UICounter::UICounter(): Component(ComponentType::C_UICounter), m_counter(0), m_isSubscribed(false) {}
UICounter::~UICounter() {}

void UICounter::DeActivate() {
	pGO = nullptr;
}

void UICounter::Update(float dt) {}

void UICounter::Serialize(const json& j) {
	std::string eventToSubscribeTo;
	eventToSubscribeTo = ParseString(j, "eventSubscription");
	if (eventToSubscribeTo == "EVENT_UIAmmoUpdate") {
		m_eventType = EVENT_UIAmmoUpdate;
		m_isSubscribed = true;
	}
	else if (eventToSubscribeTo == "EVENT_UIGoldenFeatherUpdate") {
		m_eventType = EVENT_UIGoldenFeatherUpdate;
		m_isSubscribed = true;
	}

	TETRA_EVENTS.Subscribe(m_eventType, this);
}

void UICounter::LateInitialize() {
	if (!m_pText) {
		if (pGO)
			m_pText = pGO->GetComponent<Text>(ComponentType::C_Text);
		else {
			printf("No Game Object found. UICounter component failed to operate.\n");
			return;
		}

		if (!m_pText) {
			printf("No Text component found. UICounter component failed to operate.\n");
			assert(m_pText);
			return;
		}
	}
}

void UICounter::HandleEvent(Event* pEvent) {
	if (!m_isSubscribed) return;

	if (pEvent->Type() == m_eventType) {
		CollectibleData* pData = pEvent->Data<CollectibleData>();
		if (pData) {
			m_counter = pData->m_value;
			m_pText->SetText(std::to_string(m_counter));
		}
	}
}