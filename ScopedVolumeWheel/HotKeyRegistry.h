#pragma once
#include "HotKey.h"
#include "HotKeyHandler.h"
#include <Windows.h>
#include <memory>
#include <unordered_map>

class HotKeyRegistry {
public:
    ~HotKeyRegistry();
    void registerHotKey(const HotKey& hotKey, const std::unique_ptr<HotKeyHandler> handler);
    void handle(const MSG& msg);

private:
    int nextHotKeyId = 0;
    std::unordered_map<int, HotKey> idsToHotKeys;
    std::unordered_map<HotKey, std::unique_ptr<HotKeyHandler>> handlers;
};
