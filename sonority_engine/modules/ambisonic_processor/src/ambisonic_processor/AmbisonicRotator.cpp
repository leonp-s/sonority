#include "AmbisonicRotator.h"

AmbisonicRotator::AmbisonicRotator ()
{
}

void AmbisonicRotator::prepare (const juce::dsp::ProcessSpec & spec)
{
}

void AmbisonicRotator::process (juce::dsp::ProcessContextNonReplacing<float> & processContext,
                                Vector3 cartesian)
{
    jassert (processContext.getOutputBlock ().getNumChannels () == 9);

    std::array<float, 3> spherical_coordinates = {cartesian.x, cartesian.y, cartesian.z};
    mysofa_c2s (spherical_coordinates.data ());
    float azimuth_degrees = spherical_coordinates [0];
}

void AmbisonicRotator::ApplyRotationMatrixToAudioBlock (
    juce::dsp::AudioBlock<float> & input_block,
    const AmbisonicRotator::RotationMatrix & rotation_matrix)
{
    const auto num_channels = input_block.getNumChannels ();
    const auto num_samples = input_block.getNumSamples ();

    for (int output_channel_index = 0; output_channel_index < num_channels; ++output_channel_index)
    {
        float * channel_data = input_block.getChannelPointer (output_channel_index);

        for (int sample = 0; sample < num_samples; ++sample)
        {
            float summed_output = 0.0f;
            const float * input_data = input_block.getChannelPointer (0);

            for (int input_acn_channel = 0; input_acn_channel < num_channels; ++input_acn_channel)
                summed_output += rotation_matrix [output_channel_index] [input_acn_channel] * input_data [input_acn_channel];

            channel_data [sample] = summed_output;
            input_data += num_channels;
        }
    }
}

AmbisonicRotator::RotationMatrix AmbisonicRotator::GenerateXAxisRotationMatrixACN (float angle)
{
    AmbisonicRotator::RotationMatrix acn_matrix {};

    const float cos_a = std::cos (angle);
    const float sin_a = std::sin (angle);
    const float cos_2a = std::cos (2.f * angle);
    const float sin_2a = std::sin (2.f * angle);
    const float sqrt3_over_4 = std::sqrt (3.f / 4.f);
    const float sqrt3_over_16 = std::sqrt (3.f / 16.f);

    acn_matrix [0][0] = 1.f;

    acn_matrix [1][1] = cos_a;
    acn_matrix [1][2] = -sin_a;

    acn_matrix [2][1] = sin_a;
    acn_matrix [2][2] = cos_a;

    acn_matrix [3][3] = 1.f;

    acn_matrix [4][4] = cos_a;
    acn_matrix [4][7] = -sin_a;

    acn_matrix [5][5] = cos_2a;
    acn_matrix [5][6] = -sqrt3_over_4 * sin_2a;
    acn_matrix [5][8] = -0.5f * sin_2a;

    acn_matrix [6][5] = sqrt3_over_4 * sin_2a;
    acn_matrix [6][6] = 0.25f * (1.f + 3.f * cos_2a);
    acn_matrix [6][8] = -sqrt3_over_16 * (1.f - cos_2a);

    acn_matrix [7][4] = sin_a;
    acn_matrix [7][7] = cos_a;

    acn_matrix [8][5] = 0.5f * sin_2a;
    acn_matrix [8][6] = -sqrt3_over_16 * (1.f - cos_2a);
    acn_matrix [8][8] = 0.25f * (3.f + cos_2a);

    return acn_matrix;
}

AmbisonicRotator::RotationMatrix AmbisonicRotator::GenerateYAxisRotationMatrixACN (float angle)
{
    AmbisonicRotator::RotationMatrix acn_matrix {};

    const float cos_a = std::cos (angle);
    const float sin_a = std::sin (angle);
    const float cos_2a = std::cos (2.f * angle);
    const float sin_2a = std::sin (2.f * angle);
    const float sqrt3_over_4 = std::sqrt (3.f / 4.f);
    const float sqrt3_over_16 = std::sqrt (3.f / 16.f);

    acn_matrix [0][0] = 1.f;

    acn_matrix [1][1] = 1.f;

    acn_matrix [2][2] = cos_a;
    acn_matrix [2][3] = sin_a;

    acn_matrix [3][2] = -sin_a;
    acn_matrix [3][3] = cos_a;

    acn_matrix [4][4] = cos_a;
    acn_matrix [4][5] = -sin_a;

    acn_matrix [5][4] = sin_a;
    acn_matrix [5][5] = cos_a;

    acn_matrix [6][6] = 0.25f * (1.f + 3.f * cos_2a);
    acn_matrix [6][7] = sqrt3_over_4 * sin_2a;
    acn_matrix [6][8] = sqrt3_over_16 * (1.f - cos_2a);

    acn_matrix [7][6] = -sqrt3_over_4 * sin_2a;
    acn_matrix [7][7] = cos_2a;
    acn_matrix [7][8] = 0.5f * sin_2a;

    acn_matrix [8][6] = sqrt3_over_16 * (1.f - cos_2a);
    acn_matrix [8][7] = -0.5f * sin_2a;
    acn_matrix [8][8] = 0.25f * (3.f + cos_2a);

    return acn_matrix;
}

AmbisonicRotator::RotationMatrix AmbisonicRotator::GenerateZAxisRotationMatrixACN (float angle)
{
    AmbisonicRotator::RotationMatrix acn_matrix {};

    const float cos_a = std::cos (angle);
    const float sin_a = std::sin (angle);
    const float cos_2a = std::cos (2.f * angle);
    const float sin_2a = std::sin (2.f * angle);

    acn_matrix [0][0] = 1.f;

    acn_matrix [1][1] = cos_a;
    acn_matrix [1][3] = sin_a;

    acn_matrix [2][2] = 1.f;

    acn_matrix [3][1] = -sin_a;
    acn_matrix [3][3] = cos_a;

    acn_matrix [4][4] = cos_2a;
    acn_matrix [4][8] = sin_2a;

    acn_matrix [5][5] = cos_a;
    acn_matrix [5][7] = sin_a;

    acn_matrix [6][6] = 1.f;

    acn_matrix [7][5] = -sin_a;
    acn_matrix [7][7] = cos_a;

    acn_matrix [8][4] = -sin_2a;
    acn_matrix [8][8] = cos_2a;

    return acn_matrix;
}