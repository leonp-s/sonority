#include "SofaRenderer.h"

void SofaRenderer::prepare (const juce::dsp::ProcessSpec & spec)
{
    convolver_.prepare (spec);
}

void SofaRenderer::SetFilter (juce::dsp::AudioBlock<float> hrir,
                              float left_delay,
                              float right_delay,
                              float sample_rate)
{
    left_delay_ = left_delay;
    right_delay_ = right_delay;
    juce::AudioBuffer<float> hrir_buffer {static_cast<int> (hrir.getNumChannels ()),
                                          static_cast<int> (hrir.getNumSamples ())};
    hrir.copyTo (hrir_buffer);
    buffer_transfer_.set (BufferWithSampleRate {std::move (hrir_buffer), sample_rate});
}

void SofaRenderer::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    buffer_transfer_.get (
        [&] (BufferWithSampleRate & transfer_buffer)
        {
            convolver_.loadImpulseResponse (std::move (transfer_buffer.buffer),
                                            transfer_buffer.sampleRate,
                                            juce::dsp::Convolution::Stereo::yes,
                                            juce::dsp::Convolution::Trim::no,
                                            juce::dsp::Convolution::Normalise::no);
        });

    convolver_.process (replacing);
}

void SofaRenderer::reset ()
{
    convolver_.reset ();
}