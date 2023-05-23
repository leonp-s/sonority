#pragma once
#include "SonorityRTCallback.h"
#include "UnityNativeLogger.h"
#include "audio_engine/AudioEngine.h"
#include "audio_graph/AudioGraph.h"
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

    juce::Uuid RequestCreateSource ();
    void DeleteSource (juce::Uuid source);
    void SourceDidUpdate (juce::Uuid source, VirtualSourceData source_data);

private:
    juce::AudioDeviceManager audio_device_manager_;

    AudioGraph audio_graph_;

    AudioEngine audio_engine_ {audio_graph_};
    SonorityRTCallback sonority_rt_callback_ {audio_graph_};
};

extern "C"
{
void Internal_SetLogger (DebugCallbackFuncPtr debug_callback);
Sonority * Internal_CreateSonority ();
void Internal_DestroySonority (Sonority * sonority);
void Internal_SonorityPrepare (Sonority * sonority);
void Internal_SonorityRelease (Sonority * sonority);
void Internal_RequestCreateSource (Sonority * sonority, char * source);
void Internal_DeleteSource (Sonority * sonority, const char * source);
void Internal_SourceDidUpdate (Sonority * sonority,
                               const char * source,
                               bool is_playing,
                               float volume,
                               const char * file_path,
                               float x,
                               float y,
                               float z);
}
