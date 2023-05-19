#pragma once
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <sofa_renderer/SofaFilter.h>
#include <sofa_renderer/SofaRenderer.h>

class SonorityRTCallback : public juce::AudioIODeviceCallback
{
public:
    SonorityRTCallback ();
    ~SonorityRTCallback () override = default;

    void
    audioDeviceIOCallbackWithContext (const float ** inputChannelData,
                                      int numInputChannels,
                                      float ** outputChannelData,
                                      int numOutputChannels,
                                      int numSamples,
                                      const juce::AudioIODeviceCallbackContext & context) override;
    void audioDeviceAboutToStart (juce::AudioIODevice * device) override;
    void audioDeviceStopped () override;

    void ScheduleFile ();

    std::atomic<bool> is_playing_noise_ = false;

private:
    SofaFilter sofa_filter_ {SofaFilter::OpenOptions {
        .hrtf_path = std::filesystem::path (
            "/Users/LeonPS/Documents/Development/sonority/sonority_engine/MIT_KEMAR_normal_pinna.sofa"),
        .sample_rate = 48000,
    }};
    SofaRenderer sofa_renderer_;

    juce::AudioBuffer<float> fileBuffer_;
    std::vector<int> schedule_;
};
