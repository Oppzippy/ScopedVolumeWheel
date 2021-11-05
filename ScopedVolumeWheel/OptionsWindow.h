#pragma once
#include <memory>
#include <Windows.h>
#include "SystemTrayIcon.h"

class OptionsWindow
{
public:
	OptionsWindow();
	~OptionsWindow();
	const std::unique_ptr<SystemTrayIcon>& getSystemTrayIcon();
private:
	HWND hWnd = NULL;
	std::unique_ptr<SystemTrayIcon> systemTrayIcon;
};

