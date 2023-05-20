#pragma once
#include "SonorityRTCallback.h"
#include "UnityNativeLogger.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "sonority_test_module/TestModuleClass.h"

static std::unique_ptr<UnityNativeLogger> kUnityNativeLogger;

class Sonority
{
public:
    Sonority () = default;
    ~Sonority () = default;

    void Prepare ();
    void Release ();

    void SetPlayingNoise (bool is_playing_noise);
    void PlayWavFile ();
    void UpdateSphericalCoordinates (float azimuth, float elevation);

private:
    juce::AudioDeviceManager audio_device_manager_;
    SonorityRTCallback sonority_rt_callback_;
};

extern "C"
{
void Internal_SetLogger (DebugCallbackFuncPtr debug_callback);
Sonority * Internal_CreateSonority ();
void Internal_DestroySonority (Sonority * sonority);
void Internal_SonorityPrepare (Sonority * sonority);
void Internal_SonorityRelease (Sonority * sonority);
void Internal_SonoritySetPlayingNoise (Sonority * sonority, bool is_playing_noise);
void Internal_SonorityPlayWavFile (Sonority * sonority);
void Internal_SonoritySetSphericalCoordinates (Sonority * sonority, float azimuth, float elevation);
}
