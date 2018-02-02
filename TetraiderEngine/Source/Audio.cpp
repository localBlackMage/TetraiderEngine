#include "Audio.h"
#include "TetraiderAPI.h"


Audio::Audio():Component(ComponentType::C_Audio)
{

}


Audio::~Audio()
{
}

void Audio::Update(float dt)
{
	T_AUDIO.Update(dt);
}

void Audio::Serialize(json j)
{

}
