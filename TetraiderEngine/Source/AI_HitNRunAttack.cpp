//#include "Agent.h"
//#include "AI_HitNRunAttack.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

AI_HitNRunAttack::AI_HitNRunAttack()
: AI_State(NPC_State_HitNRunAttack) {
	attackLimit = 3;
}

AI_HitNRunAttack::~AI_HitNRunAttack(){
}

void AI_HitNRunAttack::OnEnter(){
	attackCounter = 1;
	float minDistanceToPlayer = 210.0f; 
	pAgent->StopMoving();
}

void AI_HitNRunAttack::OnUpdate(float dt){
	pAgent->LookAtPlayer();
	if (pAgent->IsPlayerTooClose(minDistanceToPlayer)) {
		pAgent->ChangeState(NPC_RETREAT);
		return;
	}
	if (attackCounter > attackLimit) {
		pAgent->ChangeState(NPC_ENGAGE);
		return;
	}
	if (!pAgent->IsInAttackRange()) {
		pAgent->ChangeState(NPC_ENGAGE);
		return;
	}
	if (pAgent->UseAttack(0)) {
		attackCounter++;
	}

}

void AI_HitNRunAttack::OnExit(){
	pAgent->SetSpeedMultiplier(1.0f);
}

void AI_HitNRunAttack::HandleEvent(Event* pEvent) {

}

void AI_HitNRunAttack::Serialize(const json& j) {

}