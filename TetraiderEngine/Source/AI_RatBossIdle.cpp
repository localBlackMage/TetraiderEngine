#include <Stdafx.h>

AI_RatBossIdle::AI_RatBossIdle()
: AI_State(NPC_State_RatBossIdle) {
	idleDuration = RandomFloat(1, 3); // get 1 - 3 sec
	idledSoFar = 0.0f;
}

AI_RatBossIdle::~AI_RatBossIdle(){
}

void AI_RatBossIdle::OnEnter(){
	idleDuration = RandomFloat(1, 3); 
	idledSoFar = 0.0f;
}

void AI_RatBossIdle::OnUpdate(float dt){
	if (pAgent->IsPlayerInSight()) {
		pAgent->StopMoving();
		pAgent->ChangeState(NPC_ENGAGE);
		return;
	}
	if (idledSoFar < idleDuration) {
		idledSoFar += dt;
	}
	else {
		pAgent->ChangeState(NPC_MOVETORANDOMPOINTINZONE);
	}
}

void AI_RatBossIdle::OnExit(){
	
}


void AI_RatBossIdle::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
	case EventType::EVENT_OnTakeDamage:
		pAgent->ChangeState(NPC_ENGAGE);
	}
}

void AI_RatBossIdle::Serialize(const json& j) {

}