#pragma once
#include "audio_graph/AudioGraph.h"

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <sofa_renderer/SofaDodecRenderer.h>
#include <sofa_renderer/SofaFilter.h>
class SonorityRTCallback : public juce::AudioIODeviceCallback
{
public:
    SonorityRTCallback (AudioGraph & audio_graph);
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

private:
    AudioGraph & audio_graph_;
};
