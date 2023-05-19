#pragma once

#include "BufferTransfer.h"

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

class SofaRenderer : public juce::dsp::ProcessorBase
{
public:
    SofaRenderer () = default;
    ~SofaRenderer () override = default;

    void SetFilter (juce::dsp::AudioBlock<float> hrir,
                    float left_delay,
                    float right_delay,
                    float sample_rate);

    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

private:
    BufferTransfer buffer_transfer_;
    juce::dsp::Convolution convolver_;
    float left_delay_;
    float right_delay_;
};