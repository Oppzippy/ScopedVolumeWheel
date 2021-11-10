#include "Configuration.h"
#include <iostream>
#include <fstream>
#include <ShlObj.h>
#include <PathCch.h>
#include "toml++/toml.h"
#include "ApplicationPaths.h"
#include "Win32Exception.h"

Configuration::Configuration(const std::wstring& filePath) {
    this->filePath = filePath;
    this->read();
}

std::wstring Configuration::getMusicPlayer()
{
    return this->musicPlayer;
}

void Configuration::setMusicPlayer(std::wstring applicationName)
{
    this->musicPlayer = applicationName;
    this->save();
}

void Configuration::read() {
    try {
        auto configToml = toml::parse_file(this->filePath);
        this->musicPlayer = this->toWideChar(configToml["musicPlayer"].value_or("Spotify.exe"));
    }
    catch (toml::parse_error e) {
        // TODO see if there's an error code or something to use instead
        if (e.description() != "File could not be opened for reading") {
            // TODO wrap in exception that includes file name and line
            throw e;
        }
    }
}

void Configuration::save() {
    auto configToml = toml::table({
        { "musicPlayer", this->fromWideChar(this->musicPlayer) },
    });

    std::ofstream configStream;
    configStream.open(this->filePath);
    configStream << configToml;
    configStream.close();
}

std::string Configuration::fromWideChar(const std::wstring& str) {
    int bytesNeeded = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
    std::unique_ptr<char[]> buffer(new char[bytesNeeded]);
    WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, buffer.get(), bytesNeeded, NULL, NULL);

    return std::string(buffer.get());
}

std::wstring Configuration::toWideChar(const std::string& str) {
    int bytesNeeded = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::unique_ptr<wchar_t[]> buffer(new wchar_t[bytesNeeded]);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer.get(), bytesNeeded);

    return std::wstring(buffer.get());
}
