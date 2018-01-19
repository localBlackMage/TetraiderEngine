#include "DebugManager.h"

#define PI 3.14159265358979323

void DebugManager::DrawLine(Vector3D* pA, Vector3D* pB, DebugColor color) {
	if (!m_isDebugModeEnabled)
		return;

	DebugCommand* command = new DebugCommand();

	command->mPoints.push_back(*pA);
	command->mPoints.push_back(*pB);

	SetColor(&command->color, color);
	m_debugCommands.push_back(command);
}

void DebugManager::DrawWireRectangle(Vector3D* pA, float height, float width, DebugColor color) {
	if (!m_isDebugModeEnabled)
		return;

	DebugCommand* command = new DebugCommand();

	Vector3D p1; // bottom left
	Vector3D p2; // upper left
	Vector3D p3; // upper right
	Vector3D p4; // bottom right

	p1.x = pA->x - width*0.5f;;
	p1.y = pA->y - height*0.5f;
	p1.z = pA->z;
	p1.w = 1.0f;

	p2.x = p1.x;
	p2.y = p1.y + height;
	p2.z = pA->z;
	p2.w = 1.0f;

	p3.x = p1.x + width;
	p3.y = p2.y;
	p3.z = pA->z;
	p3.w = 1.0f;

	p4.x = p3.x;
	p4.y = p1.y;
	p4.z = pA->z;
	p4.w = 1.0f;

	command->mPoints.push_back(p1);
	command->mPoints.push_back(p2);
	command->mPoints.push_back(p3);
	command->mPoints.push_back(p4);

	SetColor(&command->color, color);
	m_debugCommands.push_back(command);
}

void DebugManager::DrawWireCircle(Vector3D* pA, float radius, DebugColor color) {
	if (!m_isDebugModeEnabled)
		return;

	DebugCommand* command = new DebugCommand();

	int count = 48;
	float division = (1 / (float)count)*PI*2.0f;
	for (int i = 0; i < count; ++i) {
		Vector3D pointA;
		pointA.x = sinf(division*i);
		pointA.y = cosf(division*i);
		pointA.Normalize(pointA);
		pointA.Mul(radius);
		pointA.z = pA->z;
		pointA.w = 1.0f;
		pointA.Add(*pA);
		command->mPoints.push_back(pointA);
	}

	SetColor(&command->color, color);
	m_debugCommands.push_back(command);
}

void DebugManager::ClearDebugCommands() {
	for (auto command : m_debugCommands) {
		delete command;
	}

	m_debugCommands.clear();
}

void DebugManager::SetColor(DB_Color* pColor, DebugColor color) {
	switch (color) {
	case DebugColor::RED:
		*pColor = red;
		break;
	case DebugColor::BLUE:
		*pColor = blue;
		break;
	case DebugColor::GREEN:
		*pColor = green;
		break;
	case DebugColor::GREY:
		*pColor = grey;
		break;
	case DebugColor::YELLOW:
		*pColor = yellow;
		break;
	case DebugColor::CYAN:
		*pColor = cyan;
		break;
	case DebugColor::WHITE:
		*pColor = white;
		break;
	case DebugColor::BLACK:
		*pColor = black;
		break;
	default:
		break;
	}
}

DebugManager::DebugManager() : m_isDebugModeEnabled(false) {
	red.r = 0xFF;
	red.g = 0x00;
	red.b = 0x00;
	red.a = 0xFF;

	blue.r = 0x00;
	blue.g = 0x00;
	blue.b = 0xFF;
	blue.a = 0xFF;

	green.r = 0x00;
	green.g = 0xFF;
	green.b = 0x00;
	green.a = 0xFF;

	grey.r = 0xC0;
	grey.g = 0xC0;
	grey.b = 0xC0;
	grey.a = 0xFF;

	yellow.r = 0xFF;
	yellow.g = 0xFF;
	yellow.b = 0x00;
	yellow.a = 0xFF;

	cyan.r = 0x00;
	cyan.g = 0xFF;
	cyan.b = 0xFF;
	cyan.a = 0xFF;

	white.r = 0xFF;
	white.g = 0xFF;
	white.b = 0xFF;
	white.a = 0xFF;

	black.r = 0x00;
	black.g = 0x00;
	black.b = 0x00;
	black.a = 0xFF;
}

DebugManager::~DebugManager() {
	for (auto command : m_debugCommands) {
		delete command;
	}

	m_debugCommands.clear();
}