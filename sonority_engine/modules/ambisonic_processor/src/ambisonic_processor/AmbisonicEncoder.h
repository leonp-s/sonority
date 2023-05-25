#pragma once
#include "audio_engine/Vector3.h"

#include <hrtf/mysofa.h>
#include <juce_dsp/juce_dsp.h>

class AmbisonicEncoder
{
public:
    void process (juce::dsp::ProcessContextNonReplacing<float> & processContext, Vector3 cartesian);

    enum Coefficients
    {
        W = 0,
        X,
        Y,
        Z,
        R,
        S,
        T,
        U,
        V
    };

    static constexpr std::array<int, 9> kFMCoefficientChannelOrder {Coefficients::W,
                                                                    Coefficients::Y,
                                                                    Coefficients::Z,
                                                                    Coefficients::X,
                                                                    Coefficients::V,
                                                                    Coefficients::T,
                                                                    Coefficients::R,
                                                                    Coefficients::S,
                                                                    Coefficients::U};

private:

    std::array<float, 9> BuildSecondOrderTable (float azimuth, float elevation);
};
