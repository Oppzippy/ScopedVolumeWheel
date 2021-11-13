#pragma once
#include <Windows.h>
#include <chrono>

class VolumeDisplay {
public:
    VolumeDisplay();
    ~VolumeDisplay();
    void show(float level) noexcept;
    void setDuration(float duration) noexcept;
    void setFadeTime(float fadeTime) noexcept;
    bool isVisible() noexcept;
    void tick() noexcept;

private:
    HWND hWnd = 0;
    float level = 0;
    long long duration = static_cast<long long>(0.5 * 1e9);
    long long fadeTime = static_cast<long long>(0.5 * 1e9);
    bool visible = false;
    std::chrono::steady_clock clock;
    std::chrono::steady_clock::time_point startTime;
    void render() noexcept;
    void renderBar() noexcept;
};
