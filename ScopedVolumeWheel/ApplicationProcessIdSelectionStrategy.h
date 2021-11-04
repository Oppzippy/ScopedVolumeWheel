#pragma once
#include "ProcessIdSelectionStrategy.h"
#include <string>

class ApplicationProcessIdSelectionStrategy : public ProcessIdSelectionStrategy
{
public:
	ApplicationProcessIdSelectionStrategy(std::wstring applicationName);
	DWORD processId();
private:
	std::wstring applicationName;
	std::wstring getFileNameOfProcess(DWORD processId);
	std::wstring getFileName(wchar_t* filePath);
};

