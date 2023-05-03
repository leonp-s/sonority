#pragma once
#include "SonorityRTCallback.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "sonority_test_module/TestModuleClass.h"

extern "C"
{
void Init ();
void Deinit ();
void SetPlayingNoise (bool is_playing_noise);
}

class Sonority
{
public:
    Sonority ();
    void init ();
    void deinit ();
    void SetPlayingNoise (bool is_playing_noise);

private:
    juce::AudioDeviceManager audio_device_manager_;
    SonorityRTCallback sonority_rt_callback_;
};