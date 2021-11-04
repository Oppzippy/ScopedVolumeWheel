#include "FocusedWindowProcessIdSelectionStrategy.h"

DWORD FocusedWindowProcessIdSelectionStrategy::processId()
{
    HWND hwnd = GetForegroundWindow();
    DWORD processId = 0;
    GetWindowThreadProcessId(hwnd, &processId);
    return processId;
}
