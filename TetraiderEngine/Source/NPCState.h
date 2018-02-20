#pragma once

#ifndef NPC_STATE_H
#define NPC_STATE_H

// Macro trick to make NPC state enums from the file NPCStateNames.h
#define REGISTER_NPC_STATE(x) x,
typedef enum
{
#include "NPCStateNames.h"
	NUM_NPC_STATES
} NPCState;
#undef REGISTER_NPC_STATE

#define REGISTER_NPC_STATE(x) #x,
static const char* NPCStateText[] =
{
#include "NPCStateNames.h"
	"Invalid"
};
#undef REGISTER_NPC_STATE

#endif
