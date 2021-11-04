#pragma once
#include <Windows.h>

class ProcessIdSelectionStrategy
{
public:
	virtual DWORD processId() = 0;
};

