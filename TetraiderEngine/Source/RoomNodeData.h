/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef ROOM_NODE_DATA_H
#define ROOM_NODE_DATA_H

class Sprite;

struct RoomNodeData {
	Sprite* spriteComponent;
	unsigned short row;
	unsigned short col;
};

#endif