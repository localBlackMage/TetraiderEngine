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
	m_isDebugModeEnabled(false),
	debugCommandCount(0)
{
}

DebugManager::~DebugManager() 
{
	ClearDebugCommands();
}

void DebugManager::DrawLine(const Vector3D& pA, const Vector3D& pB, DebugColor color) {
	if (!m_isDebugModeEnabled) return;

	if (debugCommandCount >= MaxDebugCount) {
		printf("Cannot draw anymore debug commands. Increase max limit\n");
		return;
	}

	Vector3D ab = pB - pA;

	float angle = ab.x != 0 ?
		atanf(ab.y / ab.x) * RAD_TO_DEG + ((ab.x / fabsf(ab.x) < 0.f) ? 180.f : 0.f) :
		(ab.y / fabsf(ab.y)) * 90.f;
	float length = ab.Length();

	m_debugCommands[debugCommandCount].SetCommand(
		DebugShape::S_LINE, 
		_GetColor(color), 
		pA, 
		Vector3D(0, 0, angle), 
		Vector3D(length, length, length), 
		true
	);
	++debugCommandCount;
}

void DebugManager::DrawWireRectangle(const Vector3D & pos, const Vector3D & rot, const Vector3D & scale, DebugColor color)
{
	if (!m_isDebugModeEnabled) return;

	if (debugCommandCount >= MaxDebugCount) {
		printf("Cannot draw anymore debug commands. Increase max limit\n");
		return;
	}

	m_debugCommands[debugCommandCount].SetCommand(
		DebugShape::S_RECT,
		_GetColor(color),
		pos, rot, scale, true
	);

	++debugCommandCount;
}

void DebugManager::DrawWireCircle(const Vector3D& pos, float diameter, DebugColor color) {
	if (!m_isDebugModeEnabled)
		return;

	if (debugCommandCount >= MaxDebugCount) {
		printf("Cannot draw anymore debug commands. Increase max limit\n");
		return;
	}

	m_debugCommands[debugCommandCount].SetCommand(
		DebugShape::S_CIRCLE,
		_GetColor(color), pos, Vector3D(),
		Vector3D(diameter, diameter, diameter), true
	);

	++debugCommandCount;
}

void DebugManager::DrawWireCone(const Vector3D & pos, const Vector3D & rot, float arcWidth, float radius, DebugColor color)
{
	if (!m_isDebugModeEnabled) return;

	if (debugCommandCount >= MaxDebugCount) {
		printf("Cannot draw anymore debug commands. Increase max limit\n");
		return;
	}

	m_debugCommands[debugCommandCount].SetCommand(
		DebugShape::S_CONE,
		_GetColor(color),
		pos, rot, Vector3D(arcWidth, radius, 0.f), true
	);

	++debugCommandCount;
}

void DebugManager::ClearDebugCommands()
{
	for (int i = 0; i < debugCommandCount; ++i) {
			m_debugCommands[i].active = false;
	}

	debugCommandCount = 0;
}

void DebugManager::RenderDebugCommands(GameObject* pCamera)
{
	TETRA_RENDERER._SetUpDebug(*pCamera);

	int maxIndex = debugCommandCount;
	for (int i = 0; i < maxIndex; ++i) {
		TETRA_RENDERER._RenderDebugCommand(m_debugCommands[i].shape, m_debugCommands[i].color, m_debugCommands[i].pos, m_debugCommands[i].rot, m_debugCommands[i].scale);
		m_debugCommands[i].active = false;
		--debugCommandCount;
	}
}

void DebugManager::Update()
{
	if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_F1))
		m_isDebugModeEnabled = !m_isDebugModeEnabled;
}

