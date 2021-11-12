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

    std::string fromWideChar(const std::wstring& str);
    std::wstring toWideChar(const std::string& str);
};
