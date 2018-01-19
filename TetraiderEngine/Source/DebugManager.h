#ifndef DEBUG_MANAGER_H
#define DEBUG_MANAGER_H

#include <vector>
#include "../Source/Math/Vector3D.h"


struct DB_Color {
	float r, g, b = 0;
	float a = 0;
};

enum class DebugType {
	LINE,
	WIRE_RECTANGLE,
	WIRE_CIRCLE,
};

struct DebugCommand {
	bool active = false;
	DB_Color color;
	std::vector<Vector3D> mPoints;
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
	DebugManager();
	~DebugManager();

	DB_Color red, blue, green, grey, yellow, cyan, white, black;
	void SetColor(DB_Color* pColor, DebugColor color);
public:
	DebugManager(const DebugManager &) = delete;
	void operator=(const DebugManager &) = delete;

	static DebugManager& GetInstance()
	{
		static DebugManager instance;
		return instance;
	}

	bool m_isDebugModeEnabled;

	void DrawLine(Vector3D* pA, Vector3D* pB, DebugColor color);
	void DrawWireRectangle(Vector3D* pA, float height, float width, DebugColor color);
	void DrawWireCircle(Vector3D* pA, float radius, DebugColor color);
	void ClearDebugCommands();
	std::vector<DebugCommand*> m_debugCommands;
};

#endif