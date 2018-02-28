#include "Agent.h"
#include "AI_DashingEngage.h"
#include "TetraiderAPI.h"

AI_DashingEngage::AI_DashingEngage()
	: AI_State(NPC_State_DashingEngage) {
}

AI_DashingEngage::~AI_DashingEngage() {
}

void AI_DashingEngage::OnEnter() {
	sinceEngage = 0.0f;
	engageTimer = 2.0f;
	pAgent->LookAtPlayer();
	pAgent->StopMoving();
	//play pre-dashing animation!
}

void AI_DashingEngage::OnUpdate(float dt) {
	// always face player on engage
	if (sinceEngage > engageTimer) {
		pAgent->ChangeState(NPC_ATTACK);
		return;
	}
	else if (sinceEngage < 0.75f) {
		pAgent->PlayAnimation(0); // shaking
		pAgent->MoveToPlayer();
	}
	else {
		pAgent->PlayAnimation(1); // shaking
		pAgent->StopMoving();
	}

	sinceEngage += dt;
}

void AI_DashingEngage::OnExit(){
	pAgent->PlayAnimation(0);
}

void AI_DashingEngage::HandleEvent(Event* pEvent) {

}