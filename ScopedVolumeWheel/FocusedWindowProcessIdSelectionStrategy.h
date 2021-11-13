#pragma once
#include "ProcessIdSelectionStrategy.h"

class FocusedWindowProcessIdSelectionStrategy : public ProcessIdSelectionStrategy {
public:
    DWORD processId() noexcept override;
};
