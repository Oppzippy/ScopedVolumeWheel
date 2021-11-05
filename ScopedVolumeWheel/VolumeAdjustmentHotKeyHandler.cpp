#include "VolumeAdjustmentHotKeyHandler.h"

VolumeAdjustmentHotKeyHandler::VolumeAdjustmentHotKeyHandler(
	std::shared_ptr<VolumeMixer> volumeMixer,
	std::shared_ptr<VolumeDisplay> volumeDisplay,
	std::shared_ptr<const ProcessIdSelectionStrategy> processIdSelectionStrategy,
	float adjustment
)
{
	this->volumeMixer = volumeMixer;
	this->volumeDisplay = volumeDisplay;
	this->processIdSelectionStrategy = processIdSelectionStrategy;
	this->adjustment = adjustment;
}

void VolumeAdjustmentHotKeyHandler::handle(const HotKey& hotKey)
{
	DWORD processId = this->processIdSelectionStrategy->processId();
	if (processId != 0) {
		float newVolume = volumeMixer->adjustVolumeOfProcess(processId, this->adjustment);
		if (this->volumeDisplay != nullptr) {
			this->volumeDisplay->show(newVolume);
		}
	}
}
