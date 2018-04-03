
#include <Stdafx.h>

AI_RatBossRetreat::AI_RatBossRetreat()
: AI_State(NPC_State_RatBossRetreat) {
	m_retreatFor = 0.75f;
}

AI_RatBossRetreat::~AI_RatBossRetreat(){
}

void AI_RatBossRetreat::OnEnter(){
	m_retreatTimer = 0.0f;
}

void AI_RatBossRetreat::OnUpdate(float dt){
	pAgent->LookInDirectionOfMovement();
	if (m_retreatTimer > m_retreatFor && pAgent->IsAttackAnimComplete()) {
		pAgent->ChangeState(NPC_ENGAGE);
	}		
	if (pAgent->UseAttack(0)) {
		pAgent->PlayAttackAnim();
	}
	m_retreatTimer += dt;
}

void AI_RatBossRetreat::OnExit(){
}


void AI_RatBossRetreat::HandleEvent(Event* pEvent) {

}

void AI_RatBossRetreat::Serialize(const json& j) {
}