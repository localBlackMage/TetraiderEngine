#include "Agent.h"
#include "AI_Idle.h"
#include "TetraiderAPI.h"

AI_Idle::AI_Idle()
: AI_State(NPC_State_Idle) {

}

AI_Idle::~AI_Idle(){
	pAgent = nullptr;
}

void AI_Idle::OnEnter(){

}

void AI_Idle::OnUpdate(){

}

void AI_Idle::OnExit(){
	
}