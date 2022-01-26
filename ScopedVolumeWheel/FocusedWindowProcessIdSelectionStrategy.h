#pragma once
#include "ApplicationProcessIdSelectionStrategy.h"
#include "ProcessIdSelectionStrategy.h"

class FocusedWindowProcessIdSelectionStrategy : public ProcessIdSelectionStrategy {
public:
    std::optional<ProcessSelection> processId() override;

private:
    ApplicationProcessIdSelectionStrategy fallbackStrategy;
};
