#include "VolumeAdjustmentHotKeyHandler.h"

VolumeAdjustmentHotKeyHandler::VolumeAdjustmentHotKeyHandler(
	VolumeMixer& volumeMixer,
	VolumeDisplay& volumeDisplay,
	ProcessIdSelectionStrategy& processIdSelectionStrategy,
	float adjustment
) : volumeMixer(volumeMixer), volumeDisplay(volumeDisplay), processIdSelectionStrategy(processIdSelectionStrategy)
{
	this->adjustment = adjustment;
}

void VolumeAdjustmentHotKeyHandler::handle(const HotKey& hotKey)
{
	DWORD processId = this->processIdSelectionStrategy.processId();
	if (processId != 0) {
		float newVolume = volumeMixer.adjustVolumeOfProcess(processId, this->adjustment);
		this->volumeDisplay.show(newVolume);
	}
}
