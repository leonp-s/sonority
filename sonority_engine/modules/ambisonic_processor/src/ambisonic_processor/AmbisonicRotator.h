#pragma once
#include "audio_engine/Vector3.h"

#include <hrtf/mysofa.h>
#include <juce_dsp/juce_dsp.h>

class AmbisonicRotator
{
public:
    AmbisonicRotator ();
    void prepare (const juce::dsp::ProcessSpec & spec);
    void process (juce::dsp::ProcessContextNonReplacing<float> & processContext, Vector3 cartesian);

private:
    using RotationMatrix = std::array<std::array<float, 9>, 9>;

    template <typename W,
              typename X,
              typename Y,
              typename Z,
              typename R,
              typename S,
              typename T,
              typename U,
              typename V>
    using ChannelCoefficientMap = std::map<int, float>;
    using ChannelRotationMatrix = std::array<ChannelCoefficientMap, 9>;

    static inline ChannelRotationMatrix GenerateXAxisRotationMatrixACN (float angle);
    static inline RotationMatrix GenerateYAxisRotationMatrixACN (float angle);
    static inline RotationMatrix GenerateZAxisRotationMatrixACN (float angle);

    static inline void ApplyChannelRotationMatrixToAudioBlock (
        juce::dsp::ProcessContextNonReplacing<float> process_context,
        const ChannelRotationMatrix & rotation_matrix);
};