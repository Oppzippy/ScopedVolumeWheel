#include "HotKeyRegistry.h"
#include "ExceptionWithLocation.h"
#include "Win32Exception.h"
#include "spdlog/spdlog.h";
#include <stdexcept>

HotKeyRegistry::~HotKeyRegistry()
{
    try {
        for (const auto& kv : this->idsToHotKeys) {
            const BOOL result = UnregisterHotKey(NULL, kv.first);
            throwWin32ExceptionIfNotSuccess("UnregisterHotKey", result);
        }
    } catch (const Win32Exception& e) {
        // spdlog::error isn't noexcept, suppress this warning
#pragma warning(push)
#pragma warning(disable : 26447)
        spdlog::error(e.what());
#pragma warning(pop)
    }
}

void HotKeyRegistry::registerHotKey(const HotKey& hotKey, std::unique_ptr<HotKeyHandler> handler)
{
    const BOOL result = RegisterHotKey(NULL, this->nextHotKeyId, hotKey.modifiers, hotKey.vk);
    throwWin32ExceptionIfNotSuccess("RegisterHotKey", result);
    this->idsToHotKeys[this->nextHotKeyId] = hotKey;
    this->handlers[hotKey] = std::move(handler);
    this->nextHotKeyId++;
}

void HotKeyRegistry::handle(const MSG& msg)
{
    try {
        const HotKey& hotKey = this->idsToHotKeys.at(msg.wParam);
        const std::unique_ptr<HotKeyHandler>& handler = this->handlers.at(hotKey);
        handler->handle(hotKey);
    } catch (const std::out_of_range& e) {
        throw exceptionWithLocation(ExceptionWithLocation, e.what());
    }
}
