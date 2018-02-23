#include "Agent.h"
#include "AI_Attack.h"
#include "TetraiderAPI.h"

AI_Attack::AI_Attack()
: AI_State(NPC_State_Attack) {

}

AI_Attack::~AI_Attack(){
	pAgent = nullptr;
}

void AI_Attack::OnEnter(){

}

void AI_Attack::OnUpdate(){

}

void AI_Attack::OnExit(){
	
}