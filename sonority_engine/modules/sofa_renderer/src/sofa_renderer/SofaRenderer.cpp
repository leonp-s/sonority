#include "SofaRenderer.h"

void SofaRenderer::prepare (const juce::dsp::ProcessSpec & spec)
{
    sample_rate_ = spec.sampleRate;
    convolver_.prepare (spec);
    auto mono_spec = spec;
    mono_spec.numChannels = 1;
    for (auto & delay_line : delay_lines_)
        delay_line.prepare (mono_spec);
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

    auto left_delay_samples = juce::roundToInt (left_delay_ * sample_rate_);
    auto right_delay_samples = juce::roundToInt (right_delay_ * sample_rate_);

    delay_lines_ [kLeftChannel].setMaximumDelayInSamples (left_delay_samples);
    delay_lines_ [kRightChannel].setMaximumDelayInSamples (right_delay_samples);

    delay_lines_ [kLeftChannel].setDelay (left_delay_samples);
    delay_lines_ [kRightChannel].setDelay (right_delay_samples);
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

    auto output_block = replacing.getOutputBlock ();
    auto left_block = output_block.getSingleChannelBlock (kLeftChannel);
    auto right_block = output_block.getSingleChannelBlock (kRightChannel);

    juce::dsp::ProcessContextReplacing<float> left_context (left_block);
    juce::dsp::ProcessContextReplacing<float> right_context (right_block);

    delay_lines_ [kLeftChannel].process (left_context);
    delay_lines_ [kRightChannel].process (right_context);
}

void SofaRenderer::reset ()
{
    convolver_.reset ();
    for (auto & delay_line : delay_lines_)
        delay_line.reset ();
}