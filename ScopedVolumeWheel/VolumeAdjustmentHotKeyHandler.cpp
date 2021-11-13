#include "VolumeAdjustmentHotKeyHandler.h"
#include "ProcessNotFoundException.h"
#include "spdlog/spdlog.h"

VolumeAdjustmentHotKeyHandler::VolumeAdjustmentHotKeyHandler(
    VolumeMixer& volumeMixer,
    VolumeDisplay& volumeDisplay,
    ProcessIdSelectionStrategy& processIdSelectionStrategy,
    float adjustment) noexcept
    : volumeMixer(volumeMixer)
    , volumeDisplay(volumeDisplay)
    , processIdSelectionStrategy(processIdSelectionStrategy)
{
    this->adjustment = adjustment;
}

void VolumeAdjustmentHotKeyHandler::handle(const HotKey& hotKey)
{
    const DWORD processId = this->processIdSelectionStrategy.processId();
    if (processId != 0) {
        try {
            const float newVolume = volumeMixer.adjustVolumeOfProcess(processId, this->adjustment);
            this->volumeDisplay.show(newVolume);
        } catch (ProcessNotFoundException ex) {
            spdlog::debug("Process not found when searching volume mixer sessions: {}", ex.what());
        }
    }
}
