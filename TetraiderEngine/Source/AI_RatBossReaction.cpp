#include <Stdafx.h>

AI_RatBossReaction::AI_RatBossReaction()
: AI_State(NPC_State_RatBossReaction) {

}

AI_RatBossReaction::~AI_RatBossReaction(){
}

void AI_RatBossReaction::OnEnter(){
	// play reaction animation here//
	pAgent->StopMoving();
	pAgent->LookAtPlayer();
	//pAgent->PlayReactionEffect();
	////////////////////////////////

	idleDuration = 0.75f;
	idledSoFar = 0.0f;
}

void AI_RatBossReaction::OnUpdate(float dt){
	if (idledSoFar > idleDuration) {
		pAgent->ChangeState(NPC_ATTACK);
	}
	else {
		idledSoFar += dt;
	}
}

void AI_RatBossReaction::OnExit(){
	pAgent->ControlAnimationOnVelocity(true); 
	pAgent->SetPositionBehindPlayer(170.0f);
}


void AI_RatBossReaction::HandleEvent(Event* pEvent) {

}

void AI_RatBossReaction::Serialize(const json& j) {
}