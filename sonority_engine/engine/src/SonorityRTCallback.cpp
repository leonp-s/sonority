#include "SonorityRTCallback.h"

SonorityRTCallback::SonorityRTCallback ()
{
    juce::AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats ();

    std::unique_ptr<juce::AudioFormatReader> reader (audioFormatManager.createReaderFor (
        juce::File ("/Users/micahstrange/sonority/sonority_engine/vocdemo.wav")));
    fileBuffer_.setSize (reader->numChannels, reader->lengthInSamples);
    reader->read (&fileBuffer_, 0, reader->lengthInSamples, 0, true, true);
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

    for (auto & player : schedule_)
    {
        for (auto channelIndex = 0; channelIndex < numOutputChannels; channelIndex++)
        {
            auto fileChannel = fileBuffer_.getReadPointer (channelIndex);
            auto * channel = outputChannelData [channelIndex];

            for (auto sampleIndex = 0; sampleIndex < numSamples; sampleIndex++)
                channel [sampleIndex] += fileChannel [player + sampleIndex];
        }
        player += numSamples;
    }

    std::erase_if (schedule_,
                   [&] (int player) { return player > fileBuffer_.getNumSamples () - numSamples; });
}
void SonorityRTCallback::audioDeviceAboutToStart (juce::AudioIODevice * device)
{
}
void SonorityRTCallback::audioDeviceStopped ()
{
}
void SonorityRTCallback::ScheduleFile ()
{
    schedule_.push_back (0);
}
