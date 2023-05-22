#include "WorldSpaceNode.h"

WorldSpaceNode::WorldSpaceNode (WorldSpaceNodeData world_space_node_data)
{
    UpdateNodeData (world_space_node_data);
}

void WorldSpaceNode::UpdateNodeData (WorldSpaceNodeData world_space_node_data)
{

}

void WorldSpaceNode::prepare (const juce::dsp::ProcessSpec & spec)
{
}

void WorldSpaceNode::WriteOneShot (juce::dsp::AudioBlock<float> output_block)
{
    auto output_block_length = output_block.getNumSamples ();
    auto audio_block = world_space_node_data_.audio_block_;
    auto audio_block_length = audio_block.getNumSamples ();

    if (read_position_ + output_block_length < audio_block_length)
    {
        auto write_block = audio_block.getSubBlock (read_position_, output_block_length);
        output_block.copyFrom (write_block);
    }
    else
    {
        auto remaining_samples = audio_block_length - read_position_;
        auto remaining_block = audio_block.getSubBlock (read_position_, remaining_samples);
        output_block.copyFrom (remaining_block);
    }

    read_position_ += output_block_length;
}

void WorldSpaceNode::WriteLooping (juce::dsp::AudioBlock<float> output_block)
{
    auto output_block_length = output_block.getNumSamples ();

    auto audio_block = world_space_node_data_.audio_block_;
    auto audio_block_length = audio_block.getNumSamples ();

    if (read_position_ + output_block_length < audio_block_length)
    {
        auto write_block = audio_block.getSubBlock (read_position_, output_block_length);
        output_block.copyFrom (write_block);
    }
    else
    {
        auto remaining_samples = audio_block_length - read_position_;
        auto overflowing_samples = output_block_length - remaining_samples;

        auto remaining_block = audio_block.getSubBlock (read_position_, remaining_samples);
        auto overflowing_block = audio_block.getSubBlock (0, overflowing_samples);

        output_block.copyFrom (remaining_block);
        output_block.getSubBlock (remaining_samples, overflowing_samples)
            .copyFrom (overflowing_block);
    }

    read_position_ = (read_position_ + output_block_length) % audio_block.getNumSamples ();
}

void WorldSpaceNode::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    auto output_block = replacing.getOutputBlock ();
    if (world_space_node_data_.is_looping_)
        WriteLooping (output_block);
    else
        WriteOneShot (output_block);
}

void WorldSpaceNode::reset ()
{
}
