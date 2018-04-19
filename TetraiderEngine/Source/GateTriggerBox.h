/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

struct GateInfo {
public:
	GateInfo(Vector3D _pGatePos, bool _isHorizontal) : m_pGatePos(_pGatePos), m_isHorizontal(_isHorizontal) {}
	Vector3D m_pGatePos;
	bool m_isHorizontal;
};

class GateTriggerBox : public Component {
private:
	std::vector<GateInfo> m_gatePositions;
	std::string m_gatePrefabHorizontal;
	std::string m_gatePrefabVertical;
	std::vector<GameObject*> m_gates;

	bool m_isTriggered;
	bool m_isBossTriggerBox;
public:
	GateTriggerBox();
	~GateTriggerBox();

	static Component* CreateInstance() { return new GateTriggerBox(); }

	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Deactivate();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);
	virtual void LateInitialize();
	virtual void HandleEvent(Event* pEvent);

	Vector3D GetGatePos(int index);
	int GetNumberOfGates();
	void SetGatePos(int index, const Vector3D& pos);
	void SetGatePosSize(int size);
	bool IsHorizontal(int index);
	void SetAlignment(int index, bool isHorizontal);
};

