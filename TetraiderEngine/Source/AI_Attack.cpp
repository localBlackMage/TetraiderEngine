//#include "Agent.h"
//#include "AI_Attack.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

AI_Attack::AI_Attack()
: AI_State(NPC_State_Attack) {
	attackLimit = 2;
}

AI_Attack::~AI_Attack(){
}

void AI_Attack::OnEnter(){
	attackCounter = RandomInt(1, attackLimit);
}

void AI_Attack::OnUpdate(float dt){
	pAgent->LookAtPlayer();
	if (attackCounter > attackLimit && pAgent->IsAttackAnimComplete()) {
		pAgent->ChangeState(NPC_ENGAGE);
	}
	if (pAgent->UseAttack(0)) {
		pAgent->PlayAttackAnim();
		attackCounter++;
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
}