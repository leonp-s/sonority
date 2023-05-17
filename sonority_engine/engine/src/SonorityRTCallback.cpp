#include "SonorityRTCallback.h"
void SonorityRTCallback::audioDeviceIOCallback (const float ** inputChannelData,
                                                int numInputChannels,
                                                float ** outputChannelData,
                                                int numOutputChannels,
                                                int numSamples)
{
    AudioIODeviceCallback::audioDeviceIOCallback (
        inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
}
void SonorityRTCallback::audioDeviceIOCallbackWithContext (
    const float ** inputChannelData,
    int numInputChannels,
    float ** outputChannelData,
    int numOutputChannels,
    int numSamples,
    const juce::AudioIODeviceCallbackContext & context)
{
    juce::Random random;
    for (auto channelIndex = 0; channelIndex < numOutputChannels; channelIndex++)
    {
        auto * channel = outputChannelData [channelIndex];
        for (auto sampleIndex = 0; sampleIndex < numSamples; sampleIndex++)
        {
            auto randomSample = random.nextFloat ();
            channel [sampleIndex] = is_playing_noise_ ? randomSample : 0.f;
        }
    }
}
void SonorityRTCallback::audioDeviceAboutToStart (juce::AudioIODevice * device)
{
}
void SonorityRTCallback::audioDeviceStopped ()
{
}
