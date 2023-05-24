
#include "AmbisonicRotator.h"

AmbisonicRotator::AmbisonicRotator ()
{
}

void AmbisonicRotator::prepare (const juce::dsp::ProcessSpec & spec)
{
    input_buffer_.setSize (9, spec.maximumBlockSize);
}

void AmbisonicRotator::process (juce::dsp::ProcessContextReplacing<float> & processContext,
                                Vector3 cartesian)
{
    jassert (processContext.getOutputBlock ().getNumChannels () == 9);

    juce::dsp::AudioBlock<float> input_block {input_buffer_};
    auto output_block = processContext.getOutputBlock ();

    input_block.clear ();
    input_block.add (output_block);

    auto output_channel_block = output_block.getSingleChannelBlock (1);
}

void AmbisonicRotator::processWChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ();
    auto input_block = processContext.getInputBlock ();

    auto input_block_0 = input_block.getSingleChannelBlock (0);
    output_block.addProductOf (input_block_0, 1.f);
}
void AmbisonicRotator::processYChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ();
    auto input_block = processContext.getInputBlock ();

    auto input_block_1 = input_block.getSingleChannelBlock (1);
    output_block.addProductOf (input_block_1, std::cos (azimuth));

    auto input_block_3 = input_block.getSingleChannelBlock (3);
    output_block.addProductOf (input_block_3, std::sin (azimuth));
}
void AmbisonicRotator::processZChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ();
    auto input_block = processContext.getInputBlock ();

    auto input_block_2 = input_block.getSingleChannelBlock (2);
    output_block.addProductOf (input_block_2, 1.f);
}
void AmbisonicRotator::processXChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ();
    auto input_block = processContext.getInputBlock ();

    auto input_block_1 = input_block.getSingleChannelBlock (1);
    output_block.addProductOf (input_block_1, -1.f * std::sin (azimuth));

    auto input_block_3 = input_block.getSingleChannelBlock (3);
    output_block.addProductOf (input_block_3, std::cos (azimuth));
}
void AmbisonicRotator::processVChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ();
    auto input_block = processContext.getInputBlock ();

    auto input_block_4 = input_block.getSingleChannelBlock (4);
    output_block.addProductOf (input_block_4, std::cos (2.f * azimuth));

    auto input_block_8 = input_block.getSingleChannelBlock (8);
    output_block.addProductOf (input_block_8, std::sin (2.f * azimuth));
}
void AmbisonicRotator::processTChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ();
    auto input_block = processContext.getInputBlock ();

    auto input_block_5 = input_block.getSingleChannelBlock (5);
    output_block.addProductOf (input_block_5, std::cos (azimuth));

    auto input_block_7 = input_block.getSingleChannelBlock (7);
    output_block.addProductOf (input_block_7, std::sin (azimuth));
}
void AmbisonicRotator::processRChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ();
    auto input_block = processContext.getInputBlock ();

    auto input_block_6 = input_block.getSingleChannelBlock (6);
    output_block.addProductOf (input_block_6, 1.f);
}
void AmbisonicRotator::processSChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ();
    auto input_block = processContext.getInputBlock ();

    auto input_block_5 = input_block.getSingleChannelBlock (5);
    output_block.addProductOf (input_block_5, -1.f * std::sin (azimuth));

    auto input_block_7 = input_block.getSingleChannelBlock (7);
    output_block.addProductOf (input_block_7, std::cos (azimuth));
}
void AmbisonicRotator::processUChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                                        float azimuth)
{
    auto output_block = processContext.getOutputBlock ();
    auto input_block = processContext.getInputBlock ();

    auto input_block_4 = input_block.getSingleChannelBlock (4);
    output_block.addProductOf (input_block_4, -1.f * std::sin (2.f * azimuth));

    auto input_block_8 = input_block.getSingleChannelBlock (8);
    output_block.addProductOf (input_block_8, std::cos (2.f * azimuth));
}
