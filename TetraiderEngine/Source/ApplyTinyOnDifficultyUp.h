/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

class ApplyTintOnDifficultyUp : public Component
{
private:
	float m_tintRate;
public:
	ApplyTintOnDifficultyUp();
	~ApplyTintOnDifficultyUp();
	static Component* CreateInstance() { return new ApplyTintOnDifficultyUp(); }
	// Inherited via Component
    void Update(float dt);
	void Serialize(const json & j);
	void LateInitialize();
	//----end pure virtual fn

	void HandleEvent(Event* pEvent);
	bool m_isSliderBallSeleced;
};

