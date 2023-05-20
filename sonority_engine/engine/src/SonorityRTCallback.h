#pragma once
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <sofa_renderer/SofaDodecRenderer.h>
#include <sofa_renderer/SofaFilter.h>

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

    void UpdateFilters (float azimuth, float elevation);

    std::atomic<bool> is_playing_noise_ = false;

private:
    SofaFilter sofa_filter_ {SofaFilter::OpenOptions {
        .hrtf_path = std::filesystem::path (
            "/Users/micahstrange/sonority/sonority_engine/RIEC_hrir_subject_001.sofa"),
        .sample_rate = 48000,
    }};

    SofaDodecRenderer sofa_dodec_renderer_;

    juce::AudioBuffer<float> fileBuffer_;
    std::vector<int> schedule_;
};
