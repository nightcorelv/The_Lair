#pragma once
#include <Audio.h>
#include <vector>
#include <string>
#include <unordered_map>

class Audio final
{
	static DirectX::AudioEngine* audEngine;

	DirectX::SoundEffect* soundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> effectInstanss;

public:

	static void Init();
	static void Update();
	static void Release();

	void load(const std::wstring path);
	void play(bool loop = false);
	void setVolume(float volume);	//volume range 0 to 1 float
	void stop();
	
	void pause();
	void resume();

	Audio();
	Audio(const std::wstring path);
	~Audio();
};

