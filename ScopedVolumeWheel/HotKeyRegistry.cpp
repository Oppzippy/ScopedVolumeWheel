#include "HotKeyRegistry.h"
#include <stdexcept>

HotKeyRegistry::~HotKeyRegistry()
{
	for (const auto& kv : this->idsToHotKeys) {
		UnregisterHotKey(NULL, kv.first);
	}
}

void HotKeyRegistry::registerHotKey(const HotKey& hotKey, std::unique_ptr<HotKeyHandler>& handler)
{
	RegisterHotKey(NULL, this->nextHotKeyId, hotKey.modifiers, hotKey.vk);
	this->idsToHotKeys[this->nextHotKeyId] = hotKey;
	this->handlers[hotKey] = std::move(handler);
	this->nextHotKeyId++;
}

void HotKeyRegistry::handle(const MSG& msg)
{
	try {
		HotKey& hotKey = this->idsToHotKeys.at(msg.wParam);
		std::unique_ptr<HotKeyHandler>& handler = this->handlers.at(hotKey);
		handler->handle(hotKey);
	}
	catch (std::out_of_range e) {
		// TODO throw a custom exception
		throw e;
	}
}
