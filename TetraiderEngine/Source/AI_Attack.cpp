#include "Agent.h"
#include "AI_Attack.h"
#include "TetraiderAPI.h"

AI_Attack::AI_Attack()
: AI_State(NPC_State_Attack) {
	attackCounter = 0;
	attackLimit = 2;
}

AI_Attack::~AI_Attack(){
}

void AI_Attack::OnEnter(){
}

void AI_Attack::OnUpdate(float dt){
	pAgent->LookAtPlayer();
	if (attackCounter > attackLimit) {
		pAgent->ChangeState(NPC_IDLE);
	}
	if (pAgent->UseAttack(0)) {
		attackCounter++;
	}
}

void AI_Attack::OnExit(){
	pAgent->SetSpeedMultiplier(1.0f);
}

void AI_Attack::HandleEvent(Event* pEvent) {

}