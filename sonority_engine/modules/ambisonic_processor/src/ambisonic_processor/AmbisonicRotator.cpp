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

    auto x_axis_rotation_matrix = GenerateXAxisRotationMatrixACN (azimuth_degrees);
    ApplyChannelRotationMatrixToAudioBlock (processContext, x_axis_rotation_matrix);
}

template <auto... Ns>
void AmbisonicRotator::ApplyChannelRotationMatrixToAudioBlock (
    juce::dsp::ProcessContextNonReplacing<float> process_context,
    const AmbisonicRotator::ChannelRotationMatrix<Ns...> & rotation_matrix)
{
    auto input_block = process_context.getInputBlock ();
    auto output_block = process_context.getOutputBlock ();

    const int num_channels = input_block.getNumChannels ();
    const int num_samples = input_block.getNumSamples ();

    for (int output_channel_index = 0; output_channel_index < num_channels; ++output_channel_index)
    {
        auto output_channel_block = output_block.getSingleChannelBlock (output_channel_index);
        for (const auto & channel_coefficient : std::get<output_channel_index> (rotation_matrix))
        {
            const int input_channel_index = channel_coefficient.first;
            auto input_channel_block = input_block.getSingleChannelBlock (input_channel_index);
            const float coefficient = channel_coefficient.second;

            output_block.addProductOf (input_channel_block, coefficient);
        }
    }
}

AmbisonicRotator::XAxisChannelRotationMatrix
AmbisonicRotator::GenerateXAxisRotationMatrixACN (float angle)
{
    const float cos_a = std::cos (angle);
    const float sin_a = std::sin (angle);
    const float cos_2a = std::cos (2.f * angle);
    const float sin_2a = std::sin (2.f * angle);
    const float sqrt3_over_4 = std::sqrt (3.f / 4.f);
    const float sqrt3_over_16 = std::sqrt (3.f / 16.f);

    return {ChannelCoefficients<1> {ChannelCoefficient {0, 1.f}},
            ChannelCoefficients<2> {ChannelCoefficient {1, cos_a}, ChannelCoefficient {2, -sin_a}},
            ChannelCoefficients<2> {ChannelCoefficient {1, sin_a}, ChannelCoefficient {2, cos_a}},
            ChannelCoefficients<1> {ChannelCoefficient {3, 1.f}},
            ChannelCoefficients<2> {ChannelCoefficient {4, cos_a}, ChannelCoefficient {7, -sin_a}},
            ChannelCoefficients<3> {ChannelCoefficient {5, cos_2a},
                                    ChannelCoefficient {6, -sqrt3_over_4 * sin_2a},
                                    ChannelCoefficient {8, -0.5f * sin_2a}},
            ChannelCoefficients<3> {ChannelCoefficient {5, sqrt3_over_4 * sin_2a},
                                    ChannelCoefficient {6, 0.25f * (1.f + 3.f * cos_2a)},
                                    ChannelCoefficient {8, -sqrt3_over_16 * (1.f - cos_2a)}},
            ChannelCoefficients<2> {ChannelCoefficient {4, sin_a}, ChannelCoefficient {7, cos_a}},
            ChannelCoefficients<3> {ChannelCoefficient {5, 0.5f * sin_2a},
                                    ChannelCoefficient {6, -sqrt3_over_16 * (1.f - cos_2a)},
                                    ChannelCoefficient {8, 0.25f * (3.f + cos_2a)}}};
}

// AmbisonicRotator::RotationMatrix AmbisonicRotator::GenerateYAxisRotationMatrixACN (float angle)
//{
//    AmbisonicRotator::RotationMatrix acn_matrix {};
//
//    const float cos_a = std::cos (angle);
//    const float sin_a = std::sin (angle);
//    const float cos_2a = std::cos (2.f * angle);
//    const float sin_2a = std::sin (2.f * angle);
//    const float sqrt3_over_4 = std::sqrt (3.f / 4.f);
//    const float sqrt3_over_16 = std::sqrt (3.f / 16.f);
//
//    ChannelRotationMatrix channel_rotation_matrix {};
//
//    acn_matrix [0] = ChannelCoefficientMap {{0, 1.f}};
//
//    acn_matrix [1] = ChannelCoefficientMap {{1, 1.f}};
//
//    acn_matrix [2] = ChannelCoefficientMap {{2, cos_a}, {3, sin_a}};
//
//    acn_matrix [3] = ChannelCoefficientMap {{2, -sin_a}, {3, cos_a}};
//
//    acn_matrix [4] = ChannelCoefficientMap {{4, cos_a}, {5, -sin_a}};
//
//    acn_matrix [5] = ChannelCoefficientMap {{4, sin_a}, {5, cos_a}};
//
//    acn_matrix [6] = ChannelCoefficientMap {{6, 0.25f * (1.f + 3.f * cos_2a)},
//                                            {7, sqrt3_over_4 * sin_2a},
//                                            {8, sqrt3_over_16 * (1.f - cos_2a)}};
//
//    acn_matrix [7] =
//        ChannelCoefficientMap {{6, -sqrt3_over_4 * sin_2a}, {7, cos_2a}, {8, 0.5f * sin_2a}};
//
//    acn_matrix [8] = ChannelCoefficientMap {
//        {6, sqrt3_over_16 * (1.f - cos_2a)}, {7, -0.5f * sin_2a}, {8, 0.25f * (3.f + cos_2a)}};
//
//    return acn_matrix;
//}
//
// AmbisonicRotator::RotationMatrix AmbisonicRotator::GenerateZAxisRotationMatrixACN (float angle)
//{
//    AmbisonicRotator::RotationMatrix acn_matrix {};
//
//    const float cos_a = std::cos (angle);
//    const float sin_a = std::sin (angle);
//    const float cos_2a = std::cos (2.f * angle);
//    const float sin_2a = std::sin (2.f * angle);
//
//    acn_matrix [0][0] = 1.f;
//
//    acn_matrix [1][1] = cos_a;
//    acn_matrix [1][3] = sin_a;
//
//    acn_matrix [2][2] = 1.f;
//
//    acn_matrix [3][1] = -sin_a;
//    acn_matrix [3][3] = cos_a;
//
//    acn_matrix [4][4] = cos_2a;
//    acn_matrix [4][8] = sin_2a;
//
//    acn_matrix [5][5] = cos_a;
//    acn_matrix [5][7] = sin_a;
//
//    acn_matrix [6][6] = 1.f;
//
//    acn_matrix [7][5] = -sin_a;
//    acn_matrix [7][7] = cos_a;
//
//    acn_matrix [8][4] = -sin_2a;
//    acn_matrix [8][8] = cos_2a;
//
//    return acn_matrix;
//}