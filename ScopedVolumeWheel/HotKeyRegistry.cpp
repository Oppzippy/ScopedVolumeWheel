#include "HotKeyRegistry.h"
#include <stdexcept>

HotKeyRegistry::~HotKeyRegistry()
{
	for (const auto& kv : this->idsToHotKeys) {
		UnregisterHotKey(NULL, kv.first);
	}
}

void HotKeyRegistry::registerHotKey(const HotKey& hotKey, std::shared_ptr<HotKeyHandler> handler)
{
	RegisterHotKey(NULL, this->nextHotKeyId, hotKey.modifiers, hotKey.vk);
	this->idsToHotKeys[this->nextHotKeyId] = hotKey;
	this->handlers[hotKey] = handler;
	this->nextHotKeyId++;
}

void HotKeyRegistry::handle(const MSG& msg)
{
	try {
		HotKey& hotKey = this->idsToHotKeys.at(msg.wParam);
		std::shared_ptr<HotKeyHandler> handler = this->handlers.at(hotKey);
		handler->handle(hotKey);
	}
	catch (std::out_of_range e) {
		// TODO throw a custom exception
		throw e;
	}
}
