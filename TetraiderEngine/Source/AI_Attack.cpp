#include "Agent.h"
#include "AI_Attack.h"
#include "TetraiderAPI.h"

AI_Attack::AI_Attack()
: AI_State(NPC_State_Attack) {

}

AI_Attack::~AI_Attack(){
}

void AI_Attack::OnEnter(){
	attackCounter = 0;
	attackLimit = 2;
}

void AI_Attack::OnUpdate(float dt){
	pAgent->LookAtPlayer();
	if (attackCounter > 2) {
		pAgent->ChangeState(NPC_IDLE);
	}
	if (pAgent->UseAttack(0)) {
		attackCounter++;
	}
}

void AI_Attack::OnExit(){
	
}