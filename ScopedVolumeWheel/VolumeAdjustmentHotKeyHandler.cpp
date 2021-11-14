#include "VolumeAdjustmentHotKeyHandler.h"
#include <spdlog/spdlog.h>

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
        const std::optional<float> newVolume = volumeMixer.adjustVolumeOfProcess(processId, this->adjustment);
        if (newVolume.has_value()) {
            this->volumeDisplay.show(newVolume.value());
        }
    }
}
