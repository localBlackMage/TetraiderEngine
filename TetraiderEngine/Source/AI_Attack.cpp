//#include "Agent.h"
//#include "AI_Attack.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

AI_Attack::AI_Attack()
: AI_State(NPC_State_Attack) {
	m_attackMinLimit = 1;
	m_attackMaxLimit = 2;
}

AI_Attack::~AI_Attack(){
}

void AI_Attack::OnEnter(){
	m_attackCounter = RandomInt(m_attackMinLimit, m_attackMaxLimit+1);
}

void AI_Attack::OnUpdate(float dt){
	pAgent->LookAtPlayer();
	if (m_attackCounter > m_attackMaxLimit && pAgent->IsAttackAnimComplete()) {
		pAgent->ChangeState(NPC_ENGAGE);
	}
	if (pAgent->UseAttack(0)) {
		pAgent->PlayAttackAnim();
		m_attackCounter++;
	}
	if (pAgent->IsAttackAnimComplete())
		pAgent->ControlAnimationOnVelocity(true);
}

void AI_Attack::OnExit(){
	pAgent->SetSpeedMultiplier(1.0f);
	pAgent->ControlAnimationOnVelocity(true);
}

void AI_Attack::HandleEvent(Event* pEvent) {

}

void AI_Attack::Serialize(const json& j) {
	m_attackMinLimit = ParseInt(j, "attackMinLimit");
	m_attackMaxLimit = ParseInt(j, "attackMaxLimit");
}