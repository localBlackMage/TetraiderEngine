#pragma once

#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

// Macro trick to make Component types enums from the file ComponentTypesNames.h
#define REGISTER_COMPONENT_TYPE(x) x,
typedef enum
{
#include "ComponentTypesNames.h"
	NUM_COMPONENTS
} ComponentType;
#undef REGISTER_COMPONENT_TYPE

#define REGISTER_COMPONENT_TYPE(x) #x,
static const char* ComponentTypeText[] =
{
#include "ComponentTypesNames.h"
	"Invalid"
};
#undef REGISTER_COMPONENT_TYPE

#endif // !COMPONENT_TYPES_H
