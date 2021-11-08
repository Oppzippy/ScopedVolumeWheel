#pragma once
#include <Windows.h>

class ProcessIdSelectionStrategy
{
public:
	virtual DWORD processId() const = 0;
	virtual ~ProcessIdSelectionStrategy() {};
};

