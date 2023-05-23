#include "AmbisonicEncoder.h"

#include <math.h>

static float DegreesToRadians (float degrees) noexcept
{
    return (juce::MathConstants<float>::pi / 180.0f) * degrees;
}

void AmbisonicEncoder::process (juce::dsp::ProcessContextNonReplacing<float> & processContext,
                                Vector3 cartesian)
{
    jassert (processContext.getInputBlock ().getNumChannels () == 1);
    jassert (processContext.getOutputBlock ().getNumChannels () == 9);

    std::array<float, 3> spherical_coordinates = {cartesian.x, cartesian.y, cartesian.z};
    mysofa_c2s (spherical_coordinates.data ());

    float azimuth_degrees = spherical_coordinates [0];
    float elevation_degrees = spherical_coordinates [1];

    float azimuth_rad = DegreesToRadians (azimuth_degrees);
    float elevation_rad = DegreesToRadians (elevation_degrees);

    float distance = spherical_coordinates [2];
    float distance_gain = 1.f / std::exp (std::abs (distance));
    juce::Logger::writeToLog (juce::String ("Distance Gain: ") + juce::String (distance_gain));

    auto second_order_table = BuildSecondOrderTable (azimuth_rad, elevation_rad);

    auto input_block = processContext.getInputBlock ();
    auto output_block = processContext.getOutputBlock ();

    for (auto output_channel_index = 0; output_channel_index < output_block.getNumChannels ();
         ++output_channel_index)
    {
        auto output_channel_block = output_block.getSingleChannelBlock (output_channel_index);
        auto channel_gain = second_order_table [CoefficientChannelOrder [output_channel_index]];
        for (auto input_channel_index = 0; input_channel_index < input_block.getNumChannels ();
             ++input_channel_index)
        {
            auto input_channel_block = input_block.getSingleChannelBlock (input_channel_index);
            output_channel_block.addProductOf (input_channel_block, channel_gain * distance_gain);
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
