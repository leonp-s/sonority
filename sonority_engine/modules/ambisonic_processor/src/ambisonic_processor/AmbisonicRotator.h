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

    using ChannelCoefficient = std::pair<int, float>;

    template <auto N>
    using ChannelCoefficients = std::array<ChannelCoefficient, N>;

    template <auto... Ns>
    using ChannelRotationMatrix = std::tuple<ChannelCoefficients<Ns>...>;

    using XAxisChannelRotationMatrix = ChannelRotationMatrix<1, 2, 2, 1, 2, 3, 3, 2, 3>;
    static inline XAxisChannelRotationMatrix GenerateXAxisRotationMatrixACN (float angle);

    //    static inline RotationMatrix GenerateYAxisRotationMatrixACN (float angle);
    //    static inline RotationMatrix GenerateZAxisRotationMatrixACN (float angle);

    template <auto... Ns>
    static inline void ApplyChannelRotationMatrixToAudioBlock (
        juce::dsp::ProcessContextNonReplacing<float> process_context,
        const ChannelRotationMatrix<Ns...> & rotation_matrix);
};