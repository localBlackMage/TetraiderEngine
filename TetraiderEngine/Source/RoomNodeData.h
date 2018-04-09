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