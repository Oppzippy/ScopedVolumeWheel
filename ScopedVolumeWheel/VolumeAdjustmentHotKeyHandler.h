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
		std::shared_ptr<VolumeMixer> volumeMixer,
		std::shared_ptr<VolumeDisplay> volumeDisplay,
		std::shared_ptr<const ProcessIdSelectionStrategy> processIdSelectionStrategy,
		float adjustment
	);
	virtual void handle(const HotKey& hotKey);
private:
	std::shared_ptr<const ProcessIdSelectionStrategy> processIdSelectionStrategy;
	std::shared_ptr<VolumeMixer> volumeMixer;
	std::shared_ptr<VolumeDisplay> volumeDisplay;
	float adjustment;
};

