#pragma once
#include "ProcessIdSelectionStrategy.h"
#include <string>

class ApplicationProcessIdSelectionStrategy : public ProcessIdSelectionStrategy
{
public:
	ApplicationProcessIdSelectionStrategy(std::wstring applicationName);
	virtual DWORD processId() override;
private:
	std::wstring applicationName;
	std::wstring getFileNameOfProcess(DWORD processId);
	std::wstring getFileName(const wchar_t* filePath);
};

