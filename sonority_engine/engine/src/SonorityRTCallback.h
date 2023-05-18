#pragma once
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>
#include <hrtf/mysofa.h>

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

    void ScheduleFile();

    std::atomic<bool> is_playing_noise_ = false;
private:
    juce::AudioBuffer<float> fileBuffer_;
    std::vector<int> schedule_;
};
