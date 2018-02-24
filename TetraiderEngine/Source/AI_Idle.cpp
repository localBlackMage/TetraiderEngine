#include "Agent.h"
#include "AI_Idle.h"
#include "TetraiderAPI.h"
#include "Math\MathFunctions.h"

#include <stdlib.h> // rand()
#include <iostream>

AI_Idle::AI_Idle()
: AI_State(NPC_State_Idle) {
	idleDuration = RandomInt(1, 3); // get 1 - 3 sec
	idledSoFar = 0.0f;
}

AI_Idle::~AI_Idle(){
	pAgent = nullptr;
}

void AI_Idle::OnEnter(){
	idleDuration = RandomInt(1, 3); // reset idleTime
	idledSoFar = 0.0f;
}

void AI_Idle::OnUpdate(){
	if (idledSoFar < idleDuration) {
		idledSoFar += TETRA_FRAMERATE.GetFrameTime();
	}
	else {

	}
	std::cout << RandomInt(1, 10) << endl;
	//std::cout << RandomFloat(0.1, 10) << endl;
}

void AI_Idle::OnExit(){
	
}