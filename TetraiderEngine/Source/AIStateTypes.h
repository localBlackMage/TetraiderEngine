#pragma once

#ifndef AI_STATETYPE_H
#define AI_STATETYPE_H

// Macro trick to make NPC state enums from the file NPCStateNames.h
#define REGISTER_NPC_STATE(x) x,
typedef enum
{
#include "AIStateTypeNames.h"
	NUM_NPC_STATES
} AIStateType;
#undef REGISTER_NPC_STATE

#define REGISTER_NPC_STATE(x) #x,
static const char* NPCStateText[] =
{
#include "AIStateTypeNames.h"
	"Invalid"
};
#undef REGISTER_NPC_STATE

#endif
