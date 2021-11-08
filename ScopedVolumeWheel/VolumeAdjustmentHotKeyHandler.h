#pragma once
#include <memory>
#include "HotKeyHandler.h"
#include "VolumeDisplay.h"
#include "VolumeMixer.h"
#include "ProcessIdSelectionStrategy.h"

class VolumeAdjustmentHotKeyHandler : public HotKeyHandler
{
public:
	VolumeAdjustmentHotKeyHandler(
		VolumeMixer& volumeMixer,
		VolumeDisplay& volumeDisplay,
		ProcessIdSelectionStrategy& processIdSelectionStrategy,
		float adjustment
	);
	virtual void handle(const HotKey& hotKey);
private:
	VolumeMixer& volumeMixer;
	ProcessIdSelectionStrategy& processIdSelectionStrategy;
	VolumeDisplay& volumeDisplay;
	float adjustment;
};

