#include "Body.h"
#include "Transform.h"
#include <iostream>

Body::Body() :
	Component(ComponentType::Body)
{
}

Body::~Body() {}

void Body::Update(float dt) {}

void Body::Serialize(json j) {}

void Body::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentType::Transform));
		else {
			printf("No Game Object found. Body component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Body component failed to operate.\n");
			return;
		}
	}
}