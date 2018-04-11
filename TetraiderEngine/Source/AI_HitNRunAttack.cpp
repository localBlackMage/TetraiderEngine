//#include "Agent.h"
//#include "AI_HitNRunAttack.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

AI_HitNRunAttack::AI_HitNRunAttack()
: AI_State(NPC_State_HitNRunAttack) {
	m_attackMinLimit = 1;
	m_attackMaxLimit = 3;	
	m_minDistanceToPlayer = 210.0f;
	idleDuration = 0.55;
}

AI_HitNRunAttack::~AI_HitNRunAttack(){
}

void AI_HitNRunAttack::OnEnter(){
	m_attackCounter = RandomInt(m_attackMinLimit, m_attackMaxLimit + 1);
	idleTime = 0.0f;
	pAgent->StopMoving();
}

void AI_HitNRunAttack::OnUpdate(float dt){
	pAgent->LookAtPlayer();
	if (pAgent->IsPlayerTooClose(m_minDistanceToPlayer)) {
		pAgent->ChangeState(NPC_RETREAT);
		return;
	}
	if (m_attackCounter > m_attackMaxLimit) {
		pAgent->ChangeState(NPC_ENGAGE);
		return;
	}
	if (!pAgent->IsInAttackRange()) {
		pAgent->ChangeState(NPC_ENGAGE);
		return;
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
}

void AI_HitNRunAttack::OnExit(){
	pAgent->SetSpeedMultiplier(1.0f);
}

void AI_HitNRunAttack::HandleEvent(Event* pEvent) {

}

void AI_HitNRunAttack::Serialize(const json& j) {
	m_attackMinLimit = ParseInt(j, "attackMinLimit");
	m_attackMaxLimit = ParseInt(j, "attackMaxLimit");
	m_minDistanceToPlayer = ParseFloat(j, "minDistanceToPlayer");
}