#include "GameObject.h"
#include "NPCController.h"
#include "Weapon.h"
#include "Health.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include "Camera.h"
#include <iostream>

NPCController::NPCController() :
	Agent(ComponentType::C_NPCCONTROLLER)
{
}

NPCController::~NPCController() {}

void NPCController::Update(float dt) {
	Agent::Update(dt);

}

void NPCController::Serialize(const json& j) {
	Agent::Serialize(j["AgentData"]);
}

void NPCController::HandleEvent(Event* pEvent) {
	Agent::HandleEvent(pEvent);
}

void NPCController::LateInitialize() {
	Agent::LateInitialize();

	if(!m_pWeapon) {
		if (pGO)
			m_pWeapon = pGO->GetComponent<Weapon>(ComponentType::C_Weapon);
		else {
			printf("No Game Object found. Controller component failed to operate.\n");
			return;
		}

		if (!m_pWeapon) {
			printf("No Weapon component found. Controller component failed to operate.\n");
			return;
		}
	}
}