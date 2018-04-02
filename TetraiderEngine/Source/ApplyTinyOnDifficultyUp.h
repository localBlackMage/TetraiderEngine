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

