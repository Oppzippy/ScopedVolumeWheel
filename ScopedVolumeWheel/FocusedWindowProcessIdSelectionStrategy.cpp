#include "FocusedWindowProcessIdSelectionStrategy.h"

DWORD FocusedWindowProcessIdSelectionStrategy::processId() const
{
    HWND hwnd = GetForegroundWindow();
    DWORD processId = 0;
    GetWindowThreadProcessId(hwnd, &processId);
    return processId;
}
