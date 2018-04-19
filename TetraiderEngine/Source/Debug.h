/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

enum DebugShape {
	S_CIRCLE = 0,
	S_RECT,
	S_LINE,
	S_CONE
};

struct DebugCommand {
	DebugShape shape;
	Vector3D color;
	Vector3D pos, rot, scale;
	bool active = false;
	void SetCommand(DebugShape _shape, Vector3D _color, Vector3D _pos, Vector3D _rot, Vector3D _scale, bool _active = false) {
		shape = _shape; color = _color; pos = _pos; rot = _rot; scale = _scale; active = _active;
	}
	DebugCommand() : active(false) {}
};

enum class DebugColor {
	RED,
	BLUE,
	GREEN,
	GREY,
	YELLOW,
	CYAN,
	WHITE,
	BLACK
};

#endif