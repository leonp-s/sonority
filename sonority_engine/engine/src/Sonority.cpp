#include "Sonority.h"

void Sonority::SetPlayingNoise (bool is_playing_noise)
{
    sonority_rt_callback_.is_playing_noise_ = is_playing_noise;
}

void Sonority::Prepare ()
{
    audio_device_manager_.initialiseWithDefaultDevices (0, 2);
    audio_device_manager_.addAudioCallback (&sonority_rt_callback_);
}

void Sonority::Release ()
{
    audio_device_manager_.closeAudioDevice ();
}

extern "C"
{
Sonority * Internal_CreateSonority ()
{
    return new Sonority ();
}
void Sonority::PlayWavFile ()
{
    sonority_rt_callback_.ScheduleFile ();
}

void Sonority::UpdateSphericalCoordinates (float azimuth, float elevation)
{
    sonority_rt_callback_.UpdateFilters (azimuth, elevation);
}

void Internal_DestroySonority (Sonority * sonority)
{
    delete sonority;
}

void Internal_SonoritySetPlayingNoise (Sonority * sonority, bool is_playing_noise)
{
    sonority->SetPlayingNoise (is_playing_noise);
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
    sonority->PlayWavFile ();
}

void Internal_SetSphericalCoordinates (Sonority * sonority, float azimuth, float elevation)
{
    sonority->UpdateSphericalCoordinates (azimuth, elevation);
}
}
