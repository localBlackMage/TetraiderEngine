/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ImguiManager::ImguiManager() {}

ImguiManager::~ImguiManager() {
	ImGui_ImplSdlGL3_Shutdown();
}
void ImguiManager::Initialize() {
	ImGui_ImplSdlGL3_Init(TETRA_RENDERER.GetWindow());
}

void ImguiManager::FrameStart() {
	ImGui_ImplSdlGL3_NewFrame(TETRA_RENDERER.GetWindow());
}

void ImguiManager::FrameEnd() {
	ImGui::Render();
}

void ImguiManager::HandleSDLEvents(SDL_Event &event) {
	ImGui_ImplSdlGL3_ProcessEvent(&event);
}

void ImguiManager::Update() {
	/*To do any game debug if necessary*/
}