#pragma once


class Gate : public Component {
private:
	void Init();
	void OpenGate();
	void CloseGate();
	bool m_isHorizontal;
	bool m_isInitilizeOnLevelInitialize;
	int m_numberOfGates;
	bool m_isOpen;
	Vector3D m_startOffset;
	float m_offset;
	std::vector<GameObject*> m_pGates;
	std::string m_gateSpritePrefab;
public:
	Gate();
	~Gate();

	static Component* CreateInstance() { return new Gate(); }

	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Deactivate();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);
	virtual void LateInitialize();
	virtual void HandleEvent(Event* pEvent);
};

