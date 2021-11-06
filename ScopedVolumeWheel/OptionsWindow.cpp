#include "OptionsWindow.h"
#include <strsafe.h>
#include <iostream>
#include <string>

const LPCWSTR WINDOW_CLASS_NAME = L"optionsWindow";

OptionsWindow::OptionsWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEXW wc{ 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.lpfnWndProc = [](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
		LONG_PTR selfPointer = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		OptionsWindow* self = (OptionsWindow*)selfPointer;
		
		switch (lParam) {
		case WM_RBUTTONUP:
			self->getSystemTrayIcon()->showMenu();
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	};
	wc.hInstance = hInstance;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	auto x = RegisterClassExW(&wc);

	this->hWnd = CreateWindowExW(
		WS_EX_OVERLAPPEDWINDOW,
		WINDOW_CLASS_NAME,
		L"no title",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, // Position
		CW_USEDEFAULT, CW_USEDEFAULT, // Size
		NULL,
		NULL,
		hInstance,
		NULL
	);
	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)this);
	this->systemTrayIcon = std::make_unique<SystemTrayIcon>(this->hWnd);
}

OptionsWindow::~OptionsWindow() {
	DestroyWindow(this->hWnd);
}

const std::unique_ptr<SystemTrayIcon>& OptionsWindow::getSystemTrayIcon()
{
	return this->systemTrayIcon;
}