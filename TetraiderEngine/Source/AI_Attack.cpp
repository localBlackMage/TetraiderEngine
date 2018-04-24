/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_Attack::AI_Attack()
: AI_State(NPC_State_Attack) {
	m_attackMinLimit = 1;
	m_attackMaxLimit = 2;
	idleDuration = 0.55f;
}

AI_Attack::~AI_Attack(){
}

void AI_Attack::OnEnter(){
	m_attackCounter = RandomInt(m_attackMinLimit, m_attackMaxLimit+1);
	idleTime = 0.0f;
	pAgent->StopMoving();
}

void AI_Attack::OnUpdate(float dt) {
	pAgent->LookAtPlayer();
	if (m_attackCounter > m_attackMaxLimit && pAgent->IsAttackAnimComplete()) {
		pAgent->ChangeState(NPC_ENGAGE);
	}
	if (idleTime < idleDuration) {
		idleTime += dt;
		return;
	}
		if (pAgent->UseAttack(0)) {
			pAgent->PlayAttackAnim();
			m_attackCounter++;
		}
	
	if (pAgent->IsAttackAnimComplete())
		pAgent->ControlAnimationOnVelocity(true);

	idleTime += dt;
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