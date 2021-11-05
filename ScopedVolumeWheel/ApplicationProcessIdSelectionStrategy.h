#pragma once
#include "ProcessIdSelectionStrategy.h"
#include <string>

class ApplicationProcessIdSelectionStrategy : public ProcessIdSelectionStrategy
{
public:
	ApplicationProcessIdSelectionStrategy(std::wstring applicationName);
	virtual DWORD processId() const override;
private:
	std::wstring applicationName;
	std::wstring getFileNameOfProcess(DWORD processId) const;
	std::wstring getFileName(const wchar_t* filePath) const;
};

