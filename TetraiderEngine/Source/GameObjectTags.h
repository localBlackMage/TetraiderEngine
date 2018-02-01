#pragma once

#ifndef GAME_OBJECT_TAGS_H
#define GAME_OBJECT_TAGS_H

// Macro trick to make Tags enums from the file TagNames.h
#define REGISTER_TAG_NAME(x) x,
typedef enum
{
#include "TagNames.h"

	NONE
} GameObjectTag;
#undef REGISTER_TAG_NAME

#define REGISTER_TAG_NAME(x) #x,
static const char* TagNameText[] =
{
#include "TagNames.h"
	"Invalid"
};
#undef REGISTER_TAG_NAME

#endif