/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef IMGUI_MANAGER_H
#define IMGUI_MANAGER_H

//#include "SDL_events.h"

class ImguiManager {
private:

public:
	ImguiManager();
	~ImguiManager();
	ImguiManager(const ImguiManager &) = delete;
	void operator=(const ImguiManager &) = delete;

	void Initialize();
	void FrameStart();
	void FrameEnd();
	void Update();
	void HandleSDLEvents(SDL_Event &event);
};


#endif