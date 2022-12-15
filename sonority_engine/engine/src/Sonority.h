#pragma once
#include "juce_audio_devices/juce_audio_devices.h"
#include "sonority_test_module/TestModuleClass.h"

extern "C" {
    void Init();
}

class Sonority {
public:
    Sonority();
    void init();
private:
    juce::AudioDeviceManager audio_device_manager_;
};