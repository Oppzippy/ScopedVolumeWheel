#pragma once
#include <Windows.h>
#include <optional>
#include <set>

struct ProcessSelection {
    DWORD preferredProcess;
    std::set<DWORD> fallbackProcesses;
};

class ProcessIdSelectionStrategy {
public:
    virtual std::optional<ProcessSelection> processId() = 0;
    virtual ~ProcessIdSelectionStrategy() {};
};
