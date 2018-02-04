#include "DebugManager.h"
#include "TetraiderAPI.h"
#include "GameObject.h"
#include "Math/MathLibs.h"

Vector3D DebugManager::_GetColor(DebugColor color) {
	switch (color) {
	case DebugColor::RED:
		return red;
	case DebugColor::BLUE:
		return blue;
	case DebugColor::GREEN:
		return green;
	case DebugColor::GREY:
		return grey;
	case DebugColor::YELLOW:
		return yellow;
	case DebugColor::CYAN:
		return cyan;
	case DebugColor::WHITE:
		return white;
	case DebugColor::BLACK:
		return black;
	default:
		return white;
	}
}

DebugManager::DebugManager() : 
	red(Vector3D(1, 0, 0, 1)), blue(Vector3D(0, 0, 1, 1)), 
	green(Vector3D(0, 1, 0, 1)), grey(Vector3D(.75f, .75f, .75f, 1)),
	yellow(Vector3D(1, 1, 0, 1)), cyan(Vector3D(0, 1, 1, 1)),
	white(Vector3D(1, 1, 1, 1)), black(Vector3D(0, 0, 0, 1)),
	m_isDebugModeEnabled(false) 
{
}

DebugManager::~DebugManager() 
{
	ClearDebugCommands();
}

void DebugManager::DrawLine(const Vector3D& pA, const Vector3D& pB, DebugColor color) {
	if (!m_isDebugModeEnabled) return;

	Vector3D ab = pB - pA;

	float angle = ab.x != 0 ?
		atanf(ab.y / ab.x) * RAD_TO_DEG + ((ab.x / fabsf(ab.x) < 0.f) ? 180.f : 0.f) :
		(ab.y / fabsf(ab.y)) * 90.f;
	float length = ab.Length();

	m_debugCommands.push(new DebugCommand(
		DebugShape::S_LINE,
		_GetColor(color),
		pA, Vector3D(0, 0, angle), Vector3D(length, length, length), true)
	);
}

void DebugManager::DrawWireRectangle(const Vector3D & pos, const Vector3D & rot, const Vector3D & scale, DebugColor color)
{
	if (!m_isDebugModeEnabled) return;

	m_debugCommands.push(new DebugCommand(
		DebugShape::S_RECT, 
		_GetColor(color), 
		pos, rot, scale, true)
	);
}

void DebugManager::DrawWireCircle(const Vector3D& pos, float diameter, DebugColor color) {
	if (!m_isDebugModeEnabled)
		return;

	m_debugCommands.push(new DebugCommand(
		DebugShape::S_CIRCLE,
		_GetColor(color), pos, Vector3D(), 
		Vector3D(diameter, diameter, diameter), true)
	);
}

void DebugManager::DrawWireCone(const Vector3D & pos, const Vector3D & rot, float arcWidth, float radius, DebugColor color)
{
	if (!m_isDebugModeEnabled) return;

	m_debugCommands.push(new DebugCommand(
		DebugShape::S_CONE,
		_GetColor(color),
		pos, rot, Vector3D(arcWidth, radius, 0.f), true)
	);
}

void DebugManager::ClearDebugCommands()
{
	while (!m_debugCommands.empty()) {
		m_debugCommands.pop();
	}
}

void DebugManager::RenderDebugCommands()
{
	// Example Cone
	// DrawWireCone(Vector3D(), Vector3D(0.f, 0.f, 90.f), 45.f, 200.f, DebugColor::CYAN);
	TETRA_RENDERER._SetUpDebug(*TETRA_GAME_OBJECTS.GetCamera(1));
	while (!m_debugCommands.empty()) {
		DebugCommand* debugCommand = m_debugCommands.front();
		TETRA_RENDERER._RenderDebugCommand(debugCommand->shape, debugCommand->color, debugCommand->pos, debugCommand->rot, debugCommand->scale);
		m_debugCommands.pop();
	}
}

void DebugManager::Update()
{
	if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_F1))
		m_isDebugModeEnabled = !m_isDebugModeEnabled;
}

