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

    static inline RotationMatrix GenerateXAxisRotationMatrixACN (float angle);
    static inline RotationMatrix GenerateYAxisRotationMatrixACN (float angle);
    static inline RotationMatrix GenerateZAxisRotationMatrixACN (float angle);

    static inline void ApplyRotationMatrixToAudioBlock (juce::dsp::AudioBlock<float> & input_block,
                                                        const RotationMatrix & rotation_matrix);
};