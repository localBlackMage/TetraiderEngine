#ifndef DEBUG_MANAGER_H
#define DEBUG_MANAGER_H

#include <queue>
#include "Math/Vector3D.h"
#include "Math/Matrix4x4.h"

class GameObject;

enum DebugShape {
	S_CIRCLE = 0,
	S_RECT,
	S_LINE
};

struct DebugCommand {
	DebugShape shape;
	Vector3D color;
	Vector3D pos, rot, scale;
	bool active = false;
	DebugCommand(DebugShape _shape, Vector3D _color, Vector3D _pos, Vector3D _rot, Vector3D _scale, bool _active = false) :
		shape(_shape), color(_color), pos(_pos), rot(_rot), scale(_scale), active(_active) {}

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

class DebugManager {
private:
	Vector3D red, blue, green, grey, yellow, cyan, white, black;
	std::queue<DebugCommand*> m_debugCommands;
	bool m_isDebugModeEnabled;

	Vector3D _GetColor(DebugColor color);
public:
	DebugManager();
	~DebugManager();
	DebugManager(const DebugManager &) = delete;
	void operator=(const DebugManager &) = delete;

	void SetDebugMode(bool isDebugEnabled) { m_isDebugModeEnabled = isDebugEnabled; };

	void DrawLine(const Vector3D& pA, const Vector3D& pB, DebugColor color);
	void DrawWireRectangle(const Vector3D& pA, float height, float width, DebugColor color);
	void DrawWireRectangle(const Vector3D& pos, const Vector3D& rot, const Vector3D& scale, DebugColor color);
	void DrawWireCircle(const Vector3D& pA, float radius, DebugColor color);

	void ClearDebugCommands();
	void RenderDebugCommands(const GameObject& camera);
};

#endif