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
    static constexpr int kLeftChannel = 0;
    static constexpr int kRightChannel = 1;

    float sample_rate_;

    BufferTransfer buffer_transfer_;
    juce::dsp::Convolution convolver_;

    using SofaDelayLine = juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::None>;
    std::array<SofaDelayLine, 2> delay_lines_;
    float left_delay_;
    float right_delay_;
};