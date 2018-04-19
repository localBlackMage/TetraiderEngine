/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef DEBUG_MANAGER_H
#define DEBUG_MANAGER_H

#define MaxDebugCount 5012

class DebugManager {
private:
	Vector3D red, blue, green, grey, yellow, cyan, white, black;
	int debugCommandCount;
	DebugCommand m_debugCommands[MaxDebugCount];
	bool m_isDebugModeEnabled;

	Vector3D _GetColor(DebugColor color);
public:
	DebugManager();
	~DebugManager();
	DebugManager(const DebugManager &) = delete;
	void operator=(const DebugManager &) = delete;

	void Update();
	void SetDebugMode(bool isDebugEnabled) { m_isDebugModeEnabled = isDebugEnabled; };

	void DrawLine(const Vector3D& pA, const Vector3D& pB, DebugColor color);
	void DrawWireRectangle(const Vector3D& pos, const Vector3D& rot, const Vector3D& scale, DebugColor color);
	void DrawWireCircle(const Vector3D& pA, float radius, DebugColor color);
	void DrawWireCone(const Vector3D& pos, const Vector3D& rot, float arcWidth, float radius, DebugColor color);

	void ClearDebugCommands();
	void RenderDebugCommands(GameObject* pCamera);
	bool IsDebugModeOn() { return m_isDebugModeEnabled; }
};

#endif