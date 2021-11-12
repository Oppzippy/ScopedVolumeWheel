#include "Configuration.h"
#include "ApplicationPaths.h"
#include "StringEncoding.h"
#include "Win32Exception.h"
#include "toml++/toml.h"
#include <PathCch.h>
#include <ShlObj.h>
#include <fstream>
#include <iostream>

Configuration::Configuration(const std::wstring& filePath)
{
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

void Configuration::read()
{
    try {
        auto configToml = toml::parse_file(this->filePath);
        this->musicPlayer = StringEncoding::toWideChar(configToml["musicPlayer"].value_or("Spotify.exe"));
    } catch (toml::parse_error e) {
        // TODO see if there's an error code or something to use instead
        if (e.description() != "File could not be opened for reading") {
            // TODO wrap in exception that includes file name and line
            throw e;
        }
    }
}

void Configuration::save()
{
    auto configToml = toml::table({
        { "musicPlayer", StringEncoding::fromWideChar(this->musicPlayer) },
    });

    std::ofstream configStream;
    configStream.open(this->filePath);
    configStream << configToml;
    configStream.close();
}
