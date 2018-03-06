//#include "ImguiManager.h"
//#include "SDL_events.h"
//#include "External\Imgui\imgui.h"
//#include "External\Imgui\imgui_impl_sdl_gl3.h"
//#include "TetraiderAPI.h"
//#include <iostream>

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