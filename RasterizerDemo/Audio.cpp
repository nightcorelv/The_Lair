#include "Audio.h"

DirectX::AudioEngine* Audio::audEngine;

using namespace DirectX;
void Audio::Init()
{
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{

	}

	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
	audEngine = new DirectX::AudioEngine(eflags);

}

void Audio::load(const std::wstring path)
{
	soundEffect = new SoundEffect(audEngine, path.c_str());
	effectInstanss = soundEffect->CreateInstance();
}

void Audio::play(bool loop)
{
	effectInstanss->Play(loop);
}

void Audio::setVolume(float volume)
{
	effectInstanss->SetVolume(volume);
}

void Audio::stop()
{
	effectInstanss->Stop();
}

void Audio::pause()
{
	effectInstanss->Pause();
}

void Audio::resume()
{
	effectInstanss->Resume();
}

Audio::Audio()
{

}

Audio::Audio(const std::wstring path)
{
	load(path);
}

Audio::~Audio()
{
	if(soundEffect != nullptr)
		delete soundEffect;
}

void Audio::Update()
{
	if (!audEngine->Update())
	{
		// No audio device is active
		if (audEngine->IsCriticalError())
		{

		}
	}
}

void Audio::Release()
{
	delete audEngine;
}
