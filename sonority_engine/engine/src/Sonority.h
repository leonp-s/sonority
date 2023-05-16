#pragma once
#include "SonorityRTCallback.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "sonority_test_module/TestModuleClass.h"

class Sonority
{
public:
    Sonority () = default;
    ~Sonority () = default;

    void Prepare ();
    void Release ();

    void SetPlayingNoise (bool is_playing_noise);

private:
    juce::AudioDeviceManager audio_device_manager_;
    SonorityRTCallback sonority_rt_callback_;
};

extern "C"
{
Sonority * Internal_CreateSonority ();
void Internal_DestroySonority (Sonority * sonority);
void Internal_SonorityPrepare (Sonority * sonority);
void Internal_SonorityRelease (Sonority * sonority);
void Internal_SonoritySetPlayingNoise (Sonority * sonority, bool is_playing_noise);
}
