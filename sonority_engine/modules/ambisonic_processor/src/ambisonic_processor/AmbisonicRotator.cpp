
#include "AmbisonicRotator.h"

static float DegreesToRadians (float degrees) noexcept
{
    return (juce::MathConstants<float>::pi / 180.0f) * degrees;
}

AmbisonicRotator::AmbisonicRotator ()
{
}

void AmbisonicRotator::prepare (const juce::dsp::ProcessSpec & spec)
{
    input_buffer_.setSize (9, spec.maximumBlockSize);
}

void AmbisonicRotator::process (juce::dsp::ProcessContextNonReplacing<float> & processContext,
                                Vector3 cartesian)
{
    jassert (processContext.getOutputBlock ().getNumChannels () == 9);

    std::array<float, 3> spherical_coordinates = {cartesian.x, cartesian.y, cartesian.z};

    mysofa_c2s (spherical_coordinates.data ());
    // float azimuth_degrees = spherical_coordinates [0];
    // float elevation_degrees = spherical_coordinates [1];

    if (temp_timer > 500)
    {
        azimuth_degrees = azimuth_degrees + 1.f;
        temp_timer = 0;
    }

    if (azimuth_degrees >= 360.f)
        azimuth_degrees = 0.f;
    temp_timer = temp_timer + 1;

    processWChannel (processContext, azimuth_degrees);
    processYChannel (processContext, azimuth_degrees);
    processZChannel (processContext, azimuth_degrees);
    processXChannel (processContext, azimuth_degrees);
    processVChannel (processContext, azimuth_degrees);
    processTChannel (processContext, azimuth_degrees);
    processRChannel (processContext, azimuth_degrees);
    processSChannel (processContext, azimuth_degrees);
    processUChannel (processContext, azimuth_degrees);
}

void AmbisonicRotator::processWChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ().getSingleChannelBlock (0);

    auto input_block = processContext.getInputBlock ();

    auto input_block_0 = input_block.getSingleChannelBlock (0);
    output_block.addProductOf (input_block_0, 1.f);
}
void AmbisonicRotator::processYChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ().getSingleChannelBlock (1);
    auto input_block = processContext.getInputBlock ();

    auto input_block_1 = input_block.getSingleChannelBlock (1);
    output_block.addProductOf (input_block_1, std::cos (azimuth));

    auto input_block_3 = input_block.getSingleChannelBlock (3);
    output_block.addProductOf (input_block_3, std::sin (azimuth));
}
void AmbisonicRotator::processZChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ().getSingleChannelBlock (2);
    auto input_block = processContext.getInputBlock ();

    auto input_block_2 = input_block.getSingleChannelBlock (2);
    output_block.addProductOf (input_block_2, 1.f);
}
void AmbisonicRotator::processXChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ().getSingleChannelBlock (3);
    auto input_block = processContext.getInputBlock ();

    auto input_block_1 = input_block.getSingleChannelBlock (1);
    output_block.addProductOf (input_block_1, -1.f * std::sin (azimuth));

    auto input_block_3 = input_block.getSingleChannelBlock (3);
    output_block.addProductOf (input_block_3, std::cos (azimuth));
}
void AmbisonicRotator::processVChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ().getSingleChannelBlock (4);
    auto input_block = processContext.getInputBlock ();

    auto input_block_4 = input_block.getSingleChannelBlock (4);
    output_block.addProductOf (input_block_4, std::cos (2.f * azimuth));

    auto input_block_8 = input_block.getSingleChannelBlock (8);
    output_block.addProductOf (input_block_8, std::sin (2.f * azimuth));
}
void AmbisonicRotator::processTChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ().getSingleChannelBlock (5);
    auto input_block = processContext.getInputBlock ();

    auto input_block_5 = input_block.getSingleChannelBlock (5);
    output_block.addProductOf (input_block_5, std::cos (azimuth));

    auto input_block_7 = input_block.getSingleChannelBlock (7);
    output_block.addProductOf (input_block_7, std::sin (azimuth));
}
void AmbisonicRotator::processRChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ().getSingleChannelBlock (6);
    auto input_block = processContext.getInputBlock ();

    auto input_block_6 = input_block.getSingleChannelBlock (6);
    output_block.addProductOf (input_block_6, 1.f);
}
void AmbisonicRotator::processSChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ().getSingleChannelBlock (7);
    auto input_block = processContext.getInputBlock ();

    auto input_block_5 = input_block.getSingleChannelBlock (5);
    output_block.addProductOf (input_block_5, -1.f * std::sin (azimuth));

    auto input_block_7 = input_block.getSingleChannelBlock (7);
    output_block.addProductOf (input_block_7, std::cos (azimuth));
}
void AmbisonicRotator::processUChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ().getSingleChannelBlock (8);
    auto input_block = processContext.getInputBlock ();

    auto input_block_4 = input_block.getSingleChannelBlock (4);
    output_block.addProductOf (input_block_4, -1.f * std::sin (2.f * azimuth));

    auto input_block_8 = input_block.getSingleChannelBlock (8);
    output_block.addProductOf (input_block_8, std::cos (2.f * azimuth));
}
