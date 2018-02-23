#include "Agent.h"
#include "AI_Engage.h"
#include "TetraiderAPI.h"

AI_Engage::AI_Engage()
: AI_State(NPC_State_Engage) {

}

AI_Engage::~AI_Engage(){
	pAgent = nullptr;
}

void AI_Engage::OnEnter(){

}

void AI_Engage::OnUpdate(){

}

void AI_Engage::OnExit(){
	
}