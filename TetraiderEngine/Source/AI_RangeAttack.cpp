//#include "Agent.h"
//#include "AI_Attack.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

AI_RangeAttack::AI_RangeAttack()
: AI_State(NPC_State_RangeAttack) {
	attackLimit = 2;
}

AI_RangeAttack::~AI_RangeAttack(){
}

void AI_RangeAttack::OnEnter(){
	attackCounter = 1;
}

void AI_RangeAttack::OnUpdate(float dt){
	pAgent->LookAtPlayer();
	if (attackCounter > attackLimit && pAgent->IsAttackAnimComplete()) {
		pAgent->ChangeState(NPC_ENGAGE);
	}
	if (!pAgent->IsInAttackRange()) {
		pAgent->ChangeState(NPC_ENGAGE);
		return;
	}
	if (pAgent->UseAttack(0)) {
		pAgent->PlayAttackAnim();
		attackCounter++;
	}
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
}