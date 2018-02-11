#include <stdio.h>

#include "ImguiHandler.h"
#include "TetraiderAPI.h"
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

///#include "gl3w.h"
ImguiHandler::ImguiHandler() {

	printf("ImguiHandler\n");

	///gl3wInit();
	ImGui_ImplSdlGL3_Init(TETRA_RENDERER.GetWindow());
	//
	// Setup style
	//ImGui::StyleColorsDark();
}


ImguiHandler::~ImguiHandler() {

	printf("~ImguiHandler\n");
	ShutDown();
}


void ImguiHandler::ImguiHandlerEventUpdater(SDL_Event& event) {

	ImGui_ImplSdlGL3_ProcessEvent(&event);
}

void ImguiHandler::Update() {

	ImGui_ImplSdlGL3_NewFrame(TETRA_RENDERER.GetWindow());

	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	ImGui::Render();
}

void ImguiHandler::ShutDown() {

	//ImGui_ImplSdlGL3_Shutdown();
}