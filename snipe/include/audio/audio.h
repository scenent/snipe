#pragma once
#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <thread>
#include <Windows.h>

#include "miniaudio/miniaudio.h"

class Audio {
public:
	std::wstring name;

	ma_decoder decoder{};
	ma_device_config deviceConfig{};
	ma_device device{};

	ma_engine engine{};
	std::string filePath = "";
	bool playing{ false };

	float volume = 1.0f;
public:
	Audio(std::wstring _name);
	~Audio();
	void load(std::wstring _path);
	void play();
	void playLoop();
	void playFade(float dur);
	void playLoopFade(float dur);
	void stop();
	void stopFade(float dur);
	void setVolume(float v);
};



#endif