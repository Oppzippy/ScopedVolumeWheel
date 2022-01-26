#include "FocusedWindowProcessIdSelectionStrategy.h"

std::optional<ProcessSelection> FocusedWindowProcessIdSelectionStrategy::processId()
{
    HWND hwnd = GetForegroundWindow();
    DWORD processId = 0;
    GetWindowThreadProcessId(hwnd, &processId);
    if (processId == 0) {
        return std::nullopt;
    }

    fallbackStrategy.setApplicationNameFromProcessId(processId);
    std::optional<ProcessSelection> fallbackSelection = fallbackStrategy.processId();

    ProcessSelection processSelection {
        .preferredProcess = processId,
    };

    if (fallbackSelection.has_value()) {
        processSelection.fallbackProcesses = fallbackSelection.value().fallbackProcesses;
    }

    return processSelection;
}
