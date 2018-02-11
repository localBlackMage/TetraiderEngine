#pragma once

#include "SDL_events.h"

class ImguiHandler
{

public:
	ImguiHandler();
	~ImguiHandler();

	ImguiHandler(const ImguiHandler&) = delete;
	ImguiHandler& operator=(const ImguiHandler&) = delete;


	void ImguiHandlerEventUpdater(SDL_Event & event);

	void Update();
	void ShutDown();

};

