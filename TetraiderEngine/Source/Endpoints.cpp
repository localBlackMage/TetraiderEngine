#include "Stdafx.h"
#include "Endpoints.h"


Endpoints::Endpoints():Component(ComponentType::C_Endpoints)
{
}


Endpoints::~Endpoints()
{
}

void Endpoints::Update(float dt)
{
}

void Endpoints::Serialize(const json & j)
{
}

void Endpoints::LateInitialize()
{
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. Endpoints component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Endpoints component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}

	m_start = m_pTransform->GetPosition().x - m_pTransform->GetScaleX() / 2 + 10.0f;
	m_end = m_pTransform->GetPosition().x + m_pTransform->GetScaleX() / 2 - 10.0f;

}

void Endpoints::HandleEvent(Event * pEvent)
{
}
