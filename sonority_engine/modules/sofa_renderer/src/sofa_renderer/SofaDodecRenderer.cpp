//
// Created by Micah Strange on 20/05/2023.
//

#include "SofaDodecRenderer.h"
SofaDodecRenderer::SofaDodecRenderer ()
{
    for (int buffer_index = 0; buffer_index < hrir_buffers_.size (); buffer_index++)
    {
        hrir_buffers_ [buffer_index].setSize (2, sofa_filter_.GetFilterLength ());
        sofa_filter_.GetFilterForSphericalCoordinates (hrir_buffers_ [buffer_index],
                                                       left_delays_ [buffer_index],
                                                       right_delays_ [buffer_index],
                                                       kSphericalCoordinates [buffer_index]);
    }
}
SofaDodecRenderer::~SofaDodecRenderer ()
{
}

void SofaDodecRenderer::prepare (const juce::dsp::ProcessSpec & spec)
{
    sample_rate_ = spec.sampleRate;
    for (int buffer_index = 0; buffer_index < hrir_buffers_.size (); buffer_index++)
    {
        sofa_renderers_ [buffer_index].prepare (spec);
        sofa_renderers_ [buffer_index].SetFilter (hrir_buffers_ [buffer_index],
                                                  left_delays_ [buffer_index],
                                                  right_delays_ [buffer_index],
                                                  sample_rate_);
    }

    renderer_input_buffer_.setSize (1, (int) spec.maximumBlockSize);
    renderer_output_buffer_.setSize (2, (int) spec.maximumBlockSize);
}
void SofaDodecRenderer::process (
    const juce::dsp::ProcessContextNonReplacing<float> & processContext)
{
    auto input_block = processContext.getInputBlock ();
    auto output_block = processContext.getOutputBlock ();

    jassert (input_block.getNumChannels () == 9);
    jassert (output_block.getNumChannels () == 2);

    juce::dsp::AudioBlock<float> renderer_input_block {renderer_input_buffer_};
    renderer_input_block = renderer_input_block.getSubBlock (0, input_block.getNumSamples ());

    juce::dsp::AudioBlock<float> renderer_output_block {renderer_output_buffer_};
    renderer_output_block = renderer_output_block.getSubBlock (0, output_block.getNumSamples ());

    for (auto renderer_index = 0; renderer_index < sofa_renderers_.size (); renderer_index++)
    {
        renderer_input_block.clear ();

        for (auto channel_index = 0; channel_index < input_block.getNumChannels (); ++channel_index)
        {
            auto channel = input_block.getSingleChannelBlock (channel_index);
            renderer_input_block.addProductOf (channel, kGainTable [renderer_index][channel_index]);
        }

        juce::dsp::ProcessContextNonReplacing<float> renderer_context {renderer_input_block,
                                                                       renderer_output_block};
        sofa_renderers_ [renderer_index].process (renderer_context);

        output_block.add (renderer_output_block);
    }
}
void SofaDodecRenderer::reset ()
{
    for (auto & sofa_renderer : sofa_renderers_)
        sofa_renderer.reset ();
}
