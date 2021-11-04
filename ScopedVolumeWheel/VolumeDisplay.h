#pragma once
#include <chrono>
#include <Windows.h>

class VolumeDisplay
{
public:
	VolumeDisplay();
	~VolumeDisplay();
	void show(float level);
	void setDuration(float duration);
	void setFadeTime(float fadeTime);
	bool isVisible();
	void tick();
private:
	HWND hWnd = 0;
	float level = 0;
	long long duration = 0.5 * 1e9;
	long long fadeTime = 0.5 * 1e9;
	bool visible = false;
	std::chrono::steady_clock clock;
	std::chrono::steady_clock::time_point startTime;
	void render();
	void renderBar();
};

