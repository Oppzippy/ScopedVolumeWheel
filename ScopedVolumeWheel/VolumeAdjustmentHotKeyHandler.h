#pragma once
#include "HotKeyHandler.h"
#include "ProcessIdSelectionStrategy.h"
#include "VolumeDisplay.h"
#include "VolumeMixer.h"
#include <memory>

class VolumeAdjustmentHotKeyHandler : public HotKeyHandler {
public:
    VolumeAdjustmentHotKeyHandler(
        VolumeMixer& volumeMixer,
        VolumeDisplay& volumeDisplay,
        ProcessIdSelectionStrategy& processIdSelectionStrategy,
        float adjustment) noexcept;
    ~VolumeAdjustmentHotKeyHandler() override {};
    void handle(const HotKey& hotKey) override;

private:
    VolumeMixer& volumeMixer;
    ProcessIdSelectionStrategy& processIdSelectionStrategy;
    VolumeDisplay& volumeDisplay;
    float adjustment;
};
