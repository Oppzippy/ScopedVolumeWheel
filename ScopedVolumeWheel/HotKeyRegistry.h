#pragma once
#include <unordered_map>
#include <memory>
#include <Windows.h>
#include "HotKeyHandler.h"
#include "HotKey.h"

class HotKeyRegistry
{
public:
	~HotKeyRegistry();
	void registerHotKey(const HotKey& hotKey, std::unique_ptr<HotKeyHandler>& handler);
	void handle(const MSG& msg);
private:
	int nextHotKeyId = 0;
	std::unordered_map<int, HotKey> idsToHotKeys;
	std::unordered_map<HotKey, std::unique_ptr<HotKeyHandler>> handlers;
};

