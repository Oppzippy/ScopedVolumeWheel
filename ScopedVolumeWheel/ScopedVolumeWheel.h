#pragma once
#include "HotKeyRegistry.h"
#include "VolumeMixer.h"
#include "VolumeDisplay.h"
#include "OptionsWindow.h"
#include "ProcessIdSelectionStrategy.h"

class ScopedVolumeWheel
{
public:
	ScopedVolumeWheel();
	void run();
private:
	const float VOLUME_ADJUSTMENT = 0.02f;
	std::unique_ptr<OptionsWindow> optionsWindow;
	std::unique_ptr<HotKeyRegistry> registry;
	std::unique_ptr<VolumeMixer> mixer;
	std::unique_ptr<VolumeDisplay> display;

	std::unique_ptr<ProcessIdSelectionStrategy> focusedWindowStrategy;
	std::unique_ptr<ProcessIdSelectionStrategy> musicPlayerStrategy;
};

