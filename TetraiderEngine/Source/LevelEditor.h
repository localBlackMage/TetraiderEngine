#pragma once
#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR

#include <vector>
#include <unordered_map>
#include "GameObject.h"
#include "Subscriber.h"

class Transform;

struct ObjectInstance {
	ObjectInstance(int _index, std::string _name) : index(_index), name(_name) {}
	int index;
	std::string name;
};

class LevelEditor: public Subscriber {
private:
	std::vector<std::string> m_prefabStrings;
	std::vector<int> m_instanceCount;
	std::unordered_map<GameObject*, ObjectInstance*> m_instances;
	void GoBackToMainMenu();
	GameObject* m_pSelectedGO;
	std::string m_selectedGOname;
	float m_indent = 16;
	Transform* m_pCameraTransform;
	const float m_cameraMovSpeed = 300.0f;
	Vector3D m_cameraMoveDir;
public:
	LevelEditor();
	~LevelEditor();
	LevelEditor(const LevelEditor &) = delete;
	void operator=(const LevelEditor &) = delete;

	void Update(float dt);
	void UpdateMenuBar();
	void UpdatePrefabWindow();
	void UpdateSceneWindow();
	void UpdateInspectorWindow();
	void HandleEvent(Event *pEvent);
	void Initialize();
	void DeleteInstance();
};


#endif