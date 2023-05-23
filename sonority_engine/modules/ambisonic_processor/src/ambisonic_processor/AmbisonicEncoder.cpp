#include "AmbisonicEncoder.h"

#include <math.h>

void AmbisonicEncoder::process (juce::dsp::ProcessContextNonReplacing<float> & processContext,
                                Vector3 cartesian)
{
    jassert (processContext.getInputBlock ().getNumChannels () == 1);
    jassert (processContext.getOutputBlock ().getNumChannels () == 9);

    std::array<float, 3> spherical_coordinates = {cartesian.x, cartesian.y, cartesian.z};
    mysofa_c2s (spherical_coordinates.data ());
    float azimuth = spherical_coordinates [0];
    float elevation = spherical_coordinates [1];
    float distance = spherical_coordinates [2];

    auto second_order_table = BuildSecondOrderTable (azimuth, elevation);

    auto input_block = processContext.getInputBlock ();
    auto output_block = processContext.getOutputBlock ();

    for (auto output_channel_index = 0; output_channel_index < output_block.getNumChannels ();
         ++output_channel_index)
    {
        auto output_channel_block = output_block.getSingleChannelBlock (output_channel_index);
        for (auto input_channel_index = 0; input_channel_index < input_block.getNumChannels ();
             ++input_channel_index)
        {
            auto input_channel_block = input_block.getSingleChannelBlock (input_channel_index);
            output_channel_block.addProductOf (input_channel_block,
                                               second_order_table [output_channel_index]);
        }
    }
}
std::array<float, 9> AmbisonicEncoder::BuildSecondOrderTable (float azimuth, float elevation)
{
    std::array<float, 9> gain_table;
    auto theta = azimuth;
    auto sigma = elevation;

    gain_table [Coefficients::W] = std::sqrt (2.f) / 2.f;
    gain_table [Coefficients::X] = std::cos (theta) * std::cos (sigma);
    gain_table [Coefficients::Y] = std::sin (theta) * std::cos (sigma);
    gain_table [Coefficients::Z] = std::sin (sigma);
    gain_table [Coefficients::R] = 1.5f * std::pow (std::sin (sigma), 2.f) - 0.5f;
    gain_table [Coefficients::S] = std::cos (theta) * std::sin (2.f * sigma);
    gain_table [Coefficients::T] = std::sin (theta) * std::sin (2.f * sigma);
    gain_table [Coefficients::U] = std::cos (2.f * theta) * std::pow (std::cos (sigma), 2.f);
    gain_table [Coefficients::V] = std::sin (2.f * theta) * std::pow (std::cos (sigma), 2.f);

    return gain_table;
}
