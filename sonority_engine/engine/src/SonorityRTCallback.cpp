#include "SonorityRTCallback.h"

SonorityRTCallback::SonorityRTCallback ()
{
    juce::AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats ();

    std::unique_ptr<juce::AudioFormatReader> reader (audioFormatManager.createReaderFor (
        juce::File ("/Users/micahstrange/sonority/sonority_engine/vocdemo.wav")));
    fileBuffer_.setSize (reader->numChannels, reader->lengthInSamples);
    reader->read (&fileBuffer_, 0, reader->lengthInSamples, 0, true, true);

    UpdateFilters (0.f, 0.f);
}
void SonorityRTCallback::audioDeviceIOCallbackWithContext (
    const float ** inputChannelData,
    int numInputChannels,
    float ** outputChannelData,
    int numOutputChannels,
    int numSamples,
    const juce::AudioIODeviceCallbackContext & context)
{
    juce::dsp::AudioBlock<float> ambisonic_process_block {ambisonic_buffer_};
    ambisonic_process_block.clear ();

    juce::Random random;
    for (auto channel_index = 0; channel_index < ambisonic_process_block.getNumChannels ();
         ++channel_index)
    {
        auto channel_block = ambisonic_process_block.getChannelPointer (channel_index);
        for (auto sampleIndex = 0; sampleIndex < numSamples; sampleIndex++)
        {
            auto randomSample = random.nextFloat ();
            channel_block [sampleIndex] = is_playing_noise_ ? randomSample : 0.f;
        }
    }

    //    for (auto channelIndex = 0; channelIndex < numOutputChannels; channelIndex++)
    //    {
    //        auto * channel = outputChannelData [channelIndex];
    //        for (auto sampleIndex = 0; sampleIndex < numSamples; sampleIndex++)
    //        {
    //            auto randomSample = random.nextFloat ();
    //            channel [sampleIndex] = is_playing_noise_ ? randomSample : 0.f;
    //        }
    //    }

    //    for (auto & player : schedule_)
    //    {
    //        for (auto channelIndex = 0; channelIndex < numOutputChannels; channelIndex++)
    //        {
    //            auto fileChannel = fileBuffer_.getReadPointer (channelIndex);
    //            auto * channel = outputChannelData [channelIndex];
    //
    //            for (auto sampleIndex = 0; sampleIndex < numSamples; sampleIndex++)
    //                channel [sampleIndex] += fileChannel [player + sampleIndex];
    //        }
    //        player += numSamples;
    //    }

    juce::dsp::AudioBlock<float> process_block {outputChannelData,
                                                static_cast<size_t> (numOutputChannels),
                                                static_cast<size_t> (numSamples)};

    process_block.clear ();
    juce::dsp::ProcessContextNonReplacing<float> process_context {ambisonic_process_block,
                                                                  process_block};
    sofa_dodec_renderer_.process (process_context);

    std::erase_if (schedule_,
                   [&] (int player) { return player > fileBuffer_.getNumSamples () - numSamples; });
}

void SonorityRTCallback::audioDeviceAboutToStart (juce::AudioIODevice * device)
{
    auto buffer_size = static_cast<juce::uint32> (device->getDefaultBufferSize ());
    sofa_dodec_renderer_.prepare (juce::dsp::ProcessSpec {
        .sampleRate = device->getCurrentSampleRate (),
        .maximumBlockSize = buffer_size,
        .numChannels =
            static_cast<juce::uint32> (device->getActiveOutputChannels ().toInteger ())});

    ambisonic_buffer_.setSize (9, buffer_size, false, false, false);
}

void SonorityRTCallback::audioDeviceStopped ()
{
    sofa_dodec_renderer_.reset ();
}

void SonorityRTCallback::ScheduleFile ()
{
    schedule_.push_back (0);
}

void SonorityRTCallback::UpdateFilters (float azimuth, float elevation)
{
    //    juce::AudioBuffer<float> hrir_buffer {2, sofa_filter_.GetFilterLength ()};
    //    float left_delay;
    //    float right_delay;
    //    sofa_filter_.GetFilterForSphericalCoordinates (
    //        juce::dsp::AudioBlock<float> {hrir_buffer}, left_delay, right_delay, {azimuth,
    //        elevation});
    //
    //    sofa_renderer_.SetFilter (hrir_buffer, left_delay, right_delay, 48000);
}
