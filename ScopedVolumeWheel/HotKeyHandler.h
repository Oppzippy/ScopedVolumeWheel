#pragma once
#include "HotKey.h"

class HotKeyHandler
{
public:
	virtual void handle(const HotKey& hotKey) = 0;
	virtual ~HotKeyHandler() {};
};

