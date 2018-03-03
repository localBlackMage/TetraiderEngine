#include "LevelEditor.h"
#include "External\Imgui\imgui.h"
#include "TetraiderAPI.h"
#include "Transform.h"
#include "Body.h"
#include <iostream>

LevelEditor::LevelEditor() {}

void LevelEditor::Initialize() {
	m_prefabStrings = TETRA_RESOURCES.GetPrefabStrings();
	for (unsigned int i = 0; i < m_prefabStrings.size(); ++i) {
		m_instanceCount.push_back(0);
	}

	TETRA_EVENTS.Subscribe(EVENT_INPUT_MOVE, this);
}

LevelEditor::~LevelEditor() {
	for (auto instance : m_instances) {
		delete instance.second;
	}

	m_instances.clear();
}


void LevelEditor::Update(float dt) {
	UpdateMenuBar();
	UpdatePrefabWindow();
	UpdateSceneWindow();
	UpdateInspectorWindow();

	if (m_pCameraTransform)
		m_pCameraTransform->SetPosition(m_pCameraTransform->GetPosition() + m_cameraMoveDir*m_cameraMovSpeed*dt);

	if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_DELETE)) {
		DeleteInstance();
	}
}

void LevelEditor::UpdateMenuBar() {
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Exit to menu")) GoBackToMainMenu();
		
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void LevelEditor::UpdatePrefabWindow() {
	ImGui::Begin("Prefabs");

	for (unsigned int i = 0; i < m_prefabStrings.size(); ++i) {
		if (ImGui::Selectable(m_prefabStrings[i].c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
			if (ImGui::IsMouseDoubleClicked(0)) {
				++m_instanceCount[i];
				std::string instanceString = m_prefabStrings[i] + "_" + std::to_string(m_instanceCount[i]);
				GameObject* pGO = TETRA_GAME_OBJECTS.CreateGameObject(m_prefabStrings[i]);
				ObjectInstance* pInstance = new ObjectInstance(i, instanceString);
				m_instances[pGO] = pInstance;
			}
		}
	}

	ImGui::End();
}

void LevelEditor::UpdateSceneWindow() {
	ImGui::Begin("Scene");

	for (auto instance : m_instances) {
		if (ImGui::Selectable(instance.second->name.c_str())) {
			m_pSelectedGO = instance.first;
			m_selectedGOname = instance.second->name;
		}
	}

	ImGui::End();
}

void LevelEditor::UpdateInspectorWindow() {
	ImGui::Begin("Inspector");

	if (m_pSelectedGO) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), (m_selectedGOname).c_str());
		ImGui::Indent(m_indent);
		if (ImGui::CollapsingHeader("Transform")) {
			ImGui::Indent(m_indent);
			Transform* pTransform = m_pSelectedGO->GetComponent<Transform>(C_Transform);

			if (ImGui::CollapsingHeader("Position")) {
				int x = (int)(pTransform->GetPosition().x);
				int y = (int)(pTransform->GetPosition().y);
				ImGui::InputInt("x##1", &x, 8, 64);
				ImGui::InputInt("y##1", &y, 8, 64);
				pTransform->SetPosition(Vector3D((float)x, (float)y, 0));
			}
			if (ImGui::CollapsingHeader("Scale")) {
				int x = (int)(pTransform->GetScaleX());
				int y = (int)(pTransform->GetScaleY());
				ImGui::InputInt("x##2", &x, 8, 64);
				ImGui::InputInt("y##2", &y, 8, 64);
				pTransform->SetScale((float)x, (float)y);
			}
			if (ImGui::CollapsingHeader("Rotation")) {
				int z = (int)(pTransform->GetAngleZ());
				ImGui::InputInt("z##1", &z, 5, 15);
				if (z > 360) z = 0;
				pTransform->SetAngleZ((float)z);
			}
			ImGui::Indent(-m_indent);
		}

		if (ImGui::CollapsingHeader("Body")) {
			ImGui::Indent(m_indent);
			Body* pBody = m_pSelectedGO->GetComponent<Body>(C_Body);
			if (pBody) {
				if (ImGui::CollapsingHeader("Shape")) {
					ImGui::Indent(m_indent);
					if (pBody->m_pShape->type == ShapeType::ST_AABB) {
						int width = (int)pBody->GetWidth();
						int height = (int)pBody->GetHeight();
						ImGui::InputInt("width##1", &width, 8, 64);
						ImGui::InputInt("height##1", &height, 8, 64);
						pBody->OverrideShapeData((float)width, (float)height);
					}
					else if (pBody->m_pShape->type == ShapeType::ST_Circle) {
						int radius = (int)pBody->GetRadius();
						ImGui::InputInt("radius##1", &radius, 8, 64);
						pBody->OverrideShapeData((float)radius);
					}
					ImGui::Indent(-m_indent);
				}
				if (ImGui::CollapsingHeader("Offset")) {
					ImGui::Indent(m_indent);
					int x = (int)(pBody->GetPositionOffset().x);
					int y = (int)(pBody->GetPositionOffset().y);
					ImGui::InputInt("x##3", &x, 8, 64);
					ImGui::InputInt("y##3", &y, 8, 64);
					pBody->SetOffset((float)x, (float)y);
					ImGui::Indent(-m_indent);
				}
			}
			
		}
	}

	ImGui::End();
}

void LevelEditor::GoBackToMainMenu() {
	// This is hard coded.... Change later
	TETRA_LEVELS.ChangeLevel(1);
}

void LevelEditor::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		GameObject* pGO = TETRA_GAME_OBJECTS.GetPrimaryCamera();
		if(pGO)
			m_pCameraTransform = pGO->GetComponent<Transform>(C_Transform);
	}
	else if (pEvent->Type() == EVENT_INPUT_MOVE) {
		InputAxisData* pData = pEvent->Data<InputAxisData>();
		m_cameraMoveDir = pData->m_dir;
	}
}

void LevelEditor::DeleteInstance() {
	if (!m_pSelectedGO) return;

	m_pSelectedGO->Destroy();
	std::unordered_map<GameObject*, ObjectInstance*>::iterator it = m_instances.find(m_pSelectedGO);
	--m_instanceCount[it->second->index];
	delete (it->second);
	m_instances.erase(it);
	m_pSelectedGO = nullptr;
} 