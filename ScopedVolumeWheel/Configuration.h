#pragma once
#include <string>

class Configuration {
public:
    Configuration(const std::wstring& filePath);
    std::wstring getMusicPlayer();
    void setMusicPlayer(std::wstring applicationName);
    void read();
    void save();

private:
    std::wstring filePath;
    std::wstring musicPlayer;
};
