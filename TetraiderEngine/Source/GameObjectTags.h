#pragma once

#ifndef GAME_OBJECT_TAGS_H
#define GAME_OBJECT_TAGS_H

#include <unordered_map>

// Macro trick to make Tags enums from the file TagNames.h
#define REGISTER_TAG_NAME(x) x,
typedef enum
{
#include "TagNames.h"

	T_None
} GameObjectTag;
#undef REGISTER_TAG_NAME

#define REGISTER_TAG_NAME(x) #x,
static const char* TagNameText[] =
{
#include "TagNames.h"
	"Invalid"
};
#undef REGISTER_TAG_NAME

static std::unordered_map<std::string, GameObjectTag> TAG_NAMES = {
	{ "Player", T_Player },
	{ "Camera", T_Camera },
	{ "Enemy", T_Enemy },
	{ "Projectile", T_Projectile },
	{ "None", T_None }
};

#endif