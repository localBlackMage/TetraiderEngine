#include "FlashOnTakeDamage.h"
#include "Sprite.h"

FlashOnTakeDamage::FlashOnTakeDamage(): Component(ComponentType::FlashOnTakeDamage), m_flashTime(2.0f) {}
FlashOnTakeDamage::~FlashOnTakeDamage() {}

void FlashOnTakeDamage::Update(float dt) {
	if (m_flashTime > 1)
		return;

	m_pSprite->SetColor(Lerp(m_flashColor, m_originalColor, m_flashTime));
	m_flashTime += dt*m_decaySpeed;

	if(m_flashTime > 1)
		m_pSprite->SetColor(Lerp(m_flashColor, m_originalColor, 1)); // In order not to overshoot
}

void FlashOnTakeDamage::Serialize(json j) {
	m_decaySpeed = ParseFloat(j, "decaySpeed");
	m_flashColor = ParseColor(j, "flashColor");
}

void FlashOnTakeDamage::LateInitialize() {
	if (!m_pSprite) {
		if (pGO)
			m_pSprite = static_cast<Sprite*>(pGO->GetComponent(ComponentType::Sprite));
		else {
			printf("No Game Object found. FlashOnTakeDamage component failed to operate.\n");
			return;
		}

		if (!m_pSprite) {
			printf("No Sprite component found. FlashOnTakeDamage component failed to operate.\n");
			return;
		}
	}

	m_originalColor = m_pSprite->GetColor();
}

void FlashOnTakeDamage::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EventType::EVENT_OnTakeDamage)
		Flash();
}

void FlashOnTakeDamage::Flash() {
	m_flashTime = 0.0f;
}