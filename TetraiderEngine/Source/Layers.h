/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RENDER_LAYERS_H
#define RENDER_LAYERS_H

enum RENDER_LAYER {
	L_BG_2 = 0,
	L_BG_1,
	L_BG,
	L_ONE,
	L_TWO,
	L_TREE,
	L_FOUR,
	L_UIBG,
	L_UI,
	L_UI_ONE,
	L_UI_TWO,
	L_UI_THREE,

	L_TOP,

	L_RENDER_DEBUG,

	L_NOT_RENDERED,
	L_NUM_LAYERS
};

static std::unordered_map<std::string, RENDER_LAYER> RENDER_LAYER_STRINGS = {
	{ "N/A", RENDER_LAYER::L_NOT_RENDERED },

	{ "BG_2", RENDER_LAYER::L_BG_2 },
	{ "BG_1", RENDER_LAYER::L_BG_1 },
	{ "BG", RENDER_LAYER::L_BG },
	{ "1", RENDER_LAYER::L_ONE },
	{ "2", RENDER_LAYER::L_TWO },
	{ "3", RENDER_LAYER::L_TREE },
	{ "4", RENDER_LAYER::L_FOUR },
	{ "UIBG", RENDER_LAYER::L_UIBG },
	{ "UI", RENDER_LAYER::L_UI },
	{ "UI_1", RENDER_LAYER::L_UI_ONE },
	{ "UI_2", RENDER_LAYER::L_UI_TWO },
	{ "UI_3", RENDER_LAYER::L_UI_THREE },

	{ "TOP", RENDER_LAYER::L_TOP },

	{ "DEBUG", RENDER_LAYER::L_RENDER_DEBUG }
};

#endif