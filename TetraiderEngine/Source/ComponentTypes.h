/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

static const std::string COMPONENTS = "COMPONENTS";

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
