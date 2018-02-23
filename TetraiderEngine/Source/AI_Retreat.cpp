#include "Agent.h"
#include "AI_Retreat.h"
#include "TetraiderAPI.h"

AI_Retreat::AI_Retreat()
: AI_State(NPC_State_Retreat) {

}

AI_Retreat::~AI_Retreat(){
	pAgent = nullptr;
}

void AI_Retreat::OnEnter(){

}

void AI_Retreat::OnUpdate(){

}

void AI_Retreat::OnExit(){
	
}