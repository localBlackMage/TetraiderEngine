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
	engageTimer = 3;
	//play pre-dashing animation!
}

void AI_DashingEngage::OnUpdate(float dt) {
	// always face player on engage
	pAgent->LookAtPlayer();
	if (sinceEngage > engageTimer) {
		pAgent->ChangeState(NPC_ATTACK);
		return;
	}

	sinceEngage += dt;
}

void AI_DashingEngage::OnExit(){
	
}