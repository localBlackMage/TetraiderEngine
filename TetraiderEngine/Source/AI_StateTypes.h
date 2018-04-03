#pragma once

#ifndef AI_STATETYPE_H
#define AI_STATETYPE_H

// Macro trick to make NPC state enums from the file NPCStateNames.h
#define REGISTER_STATE(x) x,
typedef enum
{
#include "AI_StateTypeNames.h"
	NUM_AI_STATES
} AIStateType;
#undef REGISTER_STATE

#define REGISTER_STATE(x) #x,
static const char* StateText[] =
{
#include "AI_StateTypeNames.h"
	"Invalid"
};
#undef REGISTER_STATE

enum NPC_CONTROLLER_AI {
	NPC_IDLE,
	NPC_REACTION,
	NPC_ENGAGE,
	NPC_ATTACK,
	NPC_RETREAT,
	NPC_MOVETORANDOMPOINTINZONE,
	NPC_STUNNED,

	NPC_NUM_BEHAVIOR
};

enum BossPhase {
	PHASE3,
	PHASE2,
	PHASE1,

	TOTAL_PHASE_NUM
};

#endif
