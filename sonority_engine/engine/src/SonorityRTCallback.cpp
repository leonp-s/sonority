#include "SonorityRTCallback.h"

void SonorityRTCallback::audioDeviceIOCallbackWithContext (
    const float ** inputChannelData,
    int numInputChannels,
    float ** outputChannelData,
    int numOutputChannels,
    int numSamples,
    const juce::AudioIODeviceCallbackContext & context)
{
    juce::dsp::AudioBlock<float> process_block {outputChannelData,
                                                static_cast<size_t> (numOutputChannels),
                                                static_cast<size_t> (numSamples)};

    process_block.clear ();
    juce::dsp::ProcessContextReplacing<float> process_context {process_block};
    audio_graph_.process (process_context);
}

void SonorityRTCallback::audioDeviceAboutToStart (juce::AudioIODevice * device)
{
    auto buffer_size = static_cast<juce::uint32> (device->getDefaultBufferSize ());
    audio_graph_.prepare (juce::dsp::ProcessSpec {
        .sampleRate = device->getCurrentSampleRate (),
        .maximumBlockSize = buffer_size,
        .numChannels =
            static_cast<juce::uint32> (device->getActiveOutputChannels ().toInteger ())});
}

void SonorityRTCallback::audioDeviceStopped ()
{
    audio_graph_.reset ();
}