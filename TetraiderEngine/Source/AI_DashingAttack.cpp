#include "Agent.h"
#include "AI_DashingAttack.h"
#include "TetraiderAPI.h"

AI_DashingAttack::AI_DashingAttack()
: AI_State(NPC_State_DashingAttack) {

}

AI_DashingAttack::~AI_DashingAttack(){
}

void AI_DashingAttack::OnEnter(){	
	pAgent->LookAtPlayer();
	pAgent->SetPositionBehindPlayer(300.0f);
}

void AI_DashingAttack::OnUpdate(float dt){

	if (pAgent->IsArrivedAtDestination()){
		pAgent->ChangeState(NPC_ENGAGE);
	}
}

void AI_DashingAttack::OnExit(){
	
}

void AI_DashingAttack::HandleEvent(Event* pEvent) {

}