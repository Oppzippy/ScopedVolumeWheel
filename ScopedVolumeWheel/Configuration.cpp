#include "Configuration.h"
#include "ApplicationPaths.h"
#include "StringEncoding.h"
#include "Win32Exception.h"
#include "spdlog/spdlog.h"
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
    } catch (const toml::parse_error& e) {
        // XXX see if there's an error code or some alternative to using the raw error message
        if (e.description() != "File could not be opened for reading") {
            // TODO log toml::parse_error::source
            spdlog::critical("Failed to load configuration file: {}", e.description());
            throw exceptionWithLocation(ExceptionWithLocation, e.what());
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
