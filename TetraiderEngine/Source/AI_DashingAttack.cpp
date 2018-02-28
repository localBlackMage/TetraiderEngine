#include "Agent.h"
#include "AI_DashingAttack.h"
#include "TetraiderAPI.h"

AI_DashingAttack::AI_DashingAttack()
: AI_State(NPC_State_DashingAttack) {

}

AI_DashingAttack::~AI_DashingAttack(){
}

void AI_DashingAttack::OnEnter(){	
	pAgent->SetPositionBehindPlayer(300.0f);
	pAgent->SetSpeedMultiplier(4.5f);
	triedSofar = 0.0f;
	tryTime = 1.5f;
}

void AI_DashingAttack::OnUpdate(float dt){
	if (triedSofar > tryTime) {
		pAgent->ChangeState(NPC_IDLE);
	}
	else {
		triedSofar += dt;
	}
	if (pAgent->IsArrivedAtDestination()){
		pAgent->ChangeState(NPC_IDLE);
	}
}

void AI_DashingAttack::OnExit(){
	pAgent->SetSpeedMultiplier(1.0f);
}

void AI_DashingAttack::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EventType::EVENT_ForceKnockBack:
			pAgent->SetVelocityToZero();
			pAgent->ChangeState(NPC_STUNNED);
	}
}