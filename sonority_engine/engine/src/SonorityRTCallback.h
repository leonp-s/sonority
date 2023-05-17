#pragma once
#include <juce_audio_devices/juce_audio_devices.h>
class SonorityRTCallback : public juce::AudioIODeviceCallback
{
public:
    ~SonorityRTCallback () override = default;
    void audioDeviceIOCallback (const float ** inputChannelData,
                                int numInputChannels,
                                float ** outputChannelData,
                                int numOutputChannels,
                                int numSamples) override;
    void
    audioDeviceIOCallbackWithContext (const float ** inputChannelData,
                                      int numInputChannels,
                                      float ** outputChannelData,
                                      int numOutputChannels,
                                      int numSamples,
                                      const juce::AudioIODeviceCallbackContext & context) override;
    void audioDeviceAboutToStart (juce::AudioIODevice * device) override;
    void audioDeviceStopped () override;

    std::atomic<bool> is_playing_noise_ = false;
};
