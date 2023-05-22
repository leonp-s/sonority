#include "Sonority.h"

void Sonority::Prepare ()
{
    audio_device_manager_.initialiseWithDefaultDevices (0, 2);
    audio_device_manager_.addAudioCallback (&sonority_rt_callback_);
}

void Sonority::Release ()
{
    audio_device_manager_.closeAudioDevice ();
    juce::Logger::setCurrentLogger (nullptr);
}

extern "C"
{
void Internal_SetLogger (DebugCallbackFuncPtr debug_callback)
{
    juce::Logger::setCurrentLogger (nullptr);
    kUnityNativeLogger = std::make_unique<UnityNativeLogger> (debug_callback);
    juce::Logger::setCurrentLogger (kUnityNativeLogger.get ());
}

Sonority * Internal_CreateSonority ()
{
    return new Sonority ();
}

void Sonority::UpdateSphericalCoordinates (float azimuth, float elevation)
{
}

void Internal_DestroySonority (Sonority * sonority)
{
    delete sonority;
}

void Internal_SonoritySetPlayingNoise (Sonority * sonority, bool is_playing_noise)
{
}

void Internal_SonorityPrepare (Sonority * sonority)
{
    sonority->Prepare ();
}

void Internal_SonorityRelease (Sonority * sonority)
{
    sonority->Release ();
}

void Internal_SonorityPlayWavFile (Sonority * sonority)
{
}

void Internal_SonoritySetSphericalCoordinates (Sonority * sonority, float azimuth, float elevation)
{
    sonority->UpdateSphericalCoordinates (azimuth, elevation);
}
}
