
#include "AmbisonicEncoder.h"
#include <math.h>

void AmbisonicEncoder::process (juce::dsp::ProcessContextNonReplacing<float> & processContext,
                                float azimuth,
                                float elevation)
{
    jassert (processContext.getInputBlock().getNumChannels() == 1);
    jassert (processContext.getOutpuBlock().getNumChannels() == 9);

    auto input_block = processContext.getInputBlock ();
    auto output_block = processContext.getOutputBlock ();

    auto channel_W = output_block.getSingleChannelBlock (0);

    channel_W = input_block.a
                * (std::sqrt (2.f)/2.f);



}
