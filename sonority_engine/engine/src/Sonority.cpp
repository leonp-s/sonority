#include "Sonority.h"

static Sonority sonority;

Sonority::Sonority ()
{
}

void Sonority::init ()
{
    // This creates audio...
    audio_device_manager_.initialiseWithDefaultDevices (0, 2);

    audio_device_manager_.addAudioCallback (&sonority_rt_callback_);
}
void Sonority::deinit ()
{
    audio_device_manager_.closeAudioDevice ();
}

extern "C"
{
void Init ()
{
    sonority.init ();
}
void Deinit ()
{
    sonority.deinit ();
}
}