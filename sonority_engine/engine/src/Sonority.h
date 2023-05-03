#pragma once
#include "SonorityRTCallback.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "sonority_test_module/TestModuleClass.h"

extern "C"
{
void Init ();
void Deinit ();
}

class Sonority
{
public:
    Sonority ();
    void init ();
    void deinit ();

private:
    juce::AudioDeviceManager audio_device_manager_;
    SonorityRTCallback sonority_rt_callback_;
};