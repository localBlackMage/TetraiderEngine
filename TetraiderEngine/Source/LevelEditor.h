#pragma once
#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR

//#include <vector>
//#include <unordered_map>
//#include "GameObject.h"
//#include "Subscriber.h"
//
//class Transform;

static const int fileNameSize = 256;
static const std::string m_gizmoObject = "P_ZGizmoDrawing";

struct ObjectInstance {
	ObjectInstance(int _index, std::string _name) : index(_index), name(_name) {}
	int index;
	std::string name;
};

class LevelEditor: public Subscriber {
private:
	std::vector<std::string> m_prefabStrings;
	std::vector<std::string> m_roomTemplatesStrings;
	std::vector<std::string> m_roomStrings;
	std::vector<int> m_instanceCount;
	std::unordered_map<GameObject*, ObjectInstance*> m_instances;
	std::string m_currentRoomType;
	char m_fileName[fileNameSize];
	void GoBackToMainMenu();
	GameObject* m_pSelectedGO;
	GameObject* m_pGizmoGO;
	std::string m_selectedGOname;
	float m_indent;
	Transform* m_pCameraTransform;
	const float m_cameraMovSpeed;
	Vector3D m_cameraMoveDir;
	int m_roomSelectedItem;
	int m_roomTemplateSelectedItem;
	bool m_isFileNameInvalid;
	bool m_isPopUpOpen;
	bool m_isLoadSuccess;

	void RoomTemplatesInit();
	void RoomInit();
	void DeleteInstance();
	void UpdateMenuBar();
	void NewPopup();
	void SavePopup();
	void LoadPopup();
	void UpdatePrefabWindow();
	GameObject* CreateInstance(int prefabNumber);
	void UpdateSceneWindow();
	void UpdateInspectorWindow();
	bool LoadJsonFile(std::string room, bool isTemplate);
	void SaveJsonFile(std::string fileName);
	void ClearScene();
	void ClearFileName();
	void SetFileName(std::string& name);
	bool ValidateFileName();
public:
	LevelEditor();
	~LevelEditor();
	LevelEditor(const LevelEditor &) = delete;
	void operator=(const LevelEditor &) = delete;

	void Update(float dt);
	void HandleEvent(Event *pEvent);
	void Initialize();

};

static bool VectorOfStringGetter(void* data, int n, const char** out_text);


#endif