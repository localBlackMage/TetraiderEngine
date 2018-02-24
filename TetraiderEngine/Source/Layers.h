#pragma once

#ifndef RENDER_LAYERS_H
#define RENDER_LAYERS_H

#include <unordered_map>
#include <string>

enum RENDER_LAYER {
	L_BG = 0,
	L_ONE,
	L_TWO,
	L_TREE,
	L_UI,
	L_UI_ONE,
	L_UI_TWO,
	L_NUM_LAYERS,
	L_NOT_RENDERED
};

static std::unordered_map<std::string, RENDER_LAYER> RENDER_LAYER_STRINGS = {
	{ "N/A", RENDER_LAYER::L_NOT_RENDERED },
	{ "BG", RENDER_LAYER::L_BG },
	{ "1", RENDER_LAYER::L_ONE },
	{ "2", RENDER_LAYER::L_TWO },
	{ "3", RENDER_LAYER::L_TREE },
	{ "UI", RENDER_LAYER::L_UI },
	{ "UI_1", RENDER_LAYER::L_UI_ONE},
	{"UI_2", RENDER_LAYER::L_UI_TWO}
};

#endif