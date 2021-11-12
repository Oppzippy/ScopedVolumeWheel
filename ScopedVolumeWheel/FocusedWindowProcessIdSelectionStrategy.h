#pragma once
#include "ProcessIdSelectionStrategy.h"

class FocusedWindowProcessIdSelectionStrategy : public ProcessIdSelectionStrategy {
public:
    virtual DWORD processId() const override;
};
