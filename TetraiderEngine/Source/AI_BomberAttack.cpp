//#include "Agent.h"
//#include "AI_BomberAttack.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

AI_BomberAttack::AI_BomberAttack()
: AI_State(NPC_State_BomberAttack) {
	m_attackMinLimit = 1;
	m_attackMaxLimit = 2;
	idleDuration = 0.55f;
}

AI_BomberAttack::~AI_BomberAttack(){
}

void AI_BomberAttack::OnEnter(){
	m_attackCounter = RandomInt(m_attackMinLimit, m_attackMaxLimit + 1);
	idleTime = 0.0f;
	pAgent->StopMoving();
}

void AI_BomberAttack::OnUpdate(float dt){
	pAgent->LookAtPlayer();
	if (m_attackCounter > m_attackMaxLimit) {
		pAgent->ChangeState(NPC_RETREAT);
	}
	if (idleTime < idleDuration) {
		idleTime += dt;
		return;
	}

	pAgent->LookAtPlayer(RandomFloat(-15, 15));
	if (pAgent->UseAttack(0)) {
		m_attackCounter++;
	}
	
	pAgent->LookAtPlayer();
	idleTime += dt;
}

void AI_BomberAttack::OnExit(){
	pAgent->SetSpeedMultiplier(1.0f);
}

void AI_BomberAttack::HandleEvent(Event* pEvent) {

}

void AI_BomberAttack::Serialize(const json& j) {
	m_attackMinLimit = ParseInt(j, "attackMinLimit");
	m_attackMaxLimit = ParseInt(j, "attackMaxLimit");
}