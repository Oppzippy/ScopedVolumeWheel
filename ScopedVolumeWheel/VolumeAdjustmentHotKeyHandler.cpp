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
    const std::optional<ProcessSelection> optionalProcessSelection = this->processIdSelectionStrategy.processId();
    if (optionalProcessSelection.has_value()) {
        const ProcessSelection& processSelection = optionalProcessSelection.value();
        std::optional<float> newVolume;
        if (processSelection.preferredProcess != 0) {
            this->volumeMixer.adjustVolumeOfProcess(processSelection.preferredProcess, this->adjustment);
        }
        if (!newVolume.has_value()) {
            newVolume = this->volumeMixer.adjustVolumeOfProcesses(processSelection.fallbackProcesses, this->adjustment);
        }
        if (newVolume.has_value()) {
            this->volumeDisplay.show(newVolume.value());
        }
    }
}
