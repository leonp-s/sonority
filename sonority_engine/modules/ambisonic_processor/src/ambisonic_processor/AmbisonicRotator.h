#pragma once
#include "AmbisonicEncoder.h"
#include "audio_engine/Vector3.h"

#include <hrtf/mysofa.h>
#include <juce_dsp/juce_dsp.h>

class AmbisonicRotator
{
public:
    AmbisonicRotator () = default;
    void process (juce::dsp::ProcessContextNonReplacing<float> & processContext, Vector3 cartesian);

private:
    using ChannelCoefficient = std::pair<int, float>;
    using ChannelCoefficients = std::array<ChannelCoefficient, 3>;
    using ChannelRotationMatrix = std::array<ChannelCoefficients, 9>;

    static inline ChannelRotationMatrix GenerateXAxisRotationMatrixACN (float angle);
    //    static inline RotationMatrix GenerateYAxisRotationMatrixACN (float angle);
    //    static inline RotationMatrix GenerateZAxisRotationMatrixACN (float angle);

    static inline void ApplyChannelRotationMatrixToAudioBlock (
        juce::dsp::ProcessContextNonReplacing<float> process_context,
        const ChannelRotationMatrix & rotation_matrix);
};