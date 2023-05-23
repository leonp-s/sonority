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

juce::Uuid Sonority::RequestCreateSource ()
{
    return audio_engine_.RequestCreateSource ();
}

void Sonority::DeleteSource (juce::Uuid source)
{
    audio_engine_.DeleteSource (source);
}

void Sonority::SourceDidUpdate (juce::Uuid source, VirtualSourceData source_data)
{
    audio_engine_.SourceDidUpdate (source, source_data);
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

void Internal_DestroySonority (Sonority * sonority)
{
    delete sonority;
}

void Internal_SonorityPrepare (Sonority * sonority)
{
    sonority->Prepare ();
}

void Internal_SonorityRelease (Sonority * sonority)
{
    sonority->Release ();
}
}

void Internal_RequestCreateSource (Sonority * sonority, char * source)
{
    auto uuid = sonority->RequestCreateSource ();
    strcpy (source, uuid.toString ().toRawUTF8 ());
}

void Internal_DeleteSource (Sonority * sonority, const char * source)
{
    auto source_uuid = juce::Uuid (source);
    sonority->DeleteSource (source_uuid);
}

void Internal_SourceDidUpdate (Sonority * sonority,
                               const char * source,
                               bool is_playing,
                               float volume,
                               const char * file_path,
                               float x,
                               float y,
                               float z)
{
    auto source_uuid = juce::Uuid (source);
    sonority->SourceDidUpdate (source_uuid,
                               VirtualSourceData {.is_playing = is_playing,
                                                  .volume = volume,
                                                  .file_path = file_path,
                                                  .cartesian = Vector3 {.x = x, .y = y, .z = z}});
}
