//#include "Agent.h"
//#include "AI_Attack.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

AI_RangeAttack::AI_RangeAttack()
: AI_State(NPC_State_RangeAttack) {
	m_attackMinLimit = 1;
	m_attackMaxLimit = 3;
	idleDuration = 0.55f;
}

AI_RangeAttack::~AI_RangeAttack(){
}

void AI_RangeAttack::OnEnter(){
	m_attackCounter = RandomInt(m_attackMinLimit, m_attackMaxLimit + 1);
	idleTime = 0.0f;
	pAgent->StopMoving();
}

void AI_RangeAttack::OnUpdate(float dt){
	pAgent->LookAtPlayer();
	if (m_attackCounter > m_attackMaxLimit && pAgent->IsAttackAnimComplete()) {
		pAgent->ChangeState(NPC_ENGAGE);
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
		pAgent->PlayAttackAnim();
		m_attackCounter++;
	}
	pAgent->LookAtPlayer();
	if (pAgent->IsAttackAnimComplete())
		pAgent->ControlAnimationOnVelocity(true);
}

void AI_RangeAttack::OnExit(){
	pAgent->SetSpeedMultiplier(1.0f);
	pAgent->ControlAnimationOnVelocity(true);
}

void AI_RangeAttack::HandleEvent(Event* pEvent) {

}

void AI_RangeAttack::Serialize(const json& j) {
	m_attackMinLimit = ParseInt(j, "attackMinLimit");
	m_attackMaxLimit = ParseInt(j, "attackMaxLimit");
}