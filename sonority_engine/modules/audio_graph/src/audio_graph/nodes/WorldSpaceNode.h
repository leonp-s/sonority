#pragma once
#include <juce_dsp/juce_dsp.h>

struct Vector3
{
    float x;
    float y;
    float z;
};

struct WorldSpaceNodeData
{
    bool is_looping_;
    float volume_;
    Vector3 coordinates_;
    juce::dsp::AudioBlock<const float> audio_block_;
};

class WorldSpaceNode, juce::dsp::ProcessorBase
{
public:
    WorldSpaceNode (WorldSpaceNodeData world_space_node_data);
    ~WorldSpaceNode () override = default;

    void UpdateNodeData (WorldSpaceNodeData world_space_node_data);

    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

private:
    void WriteOneShot (juce::dsp::AudioBlock<float> output_block);
    void WriteLooping (juce::dsp::AudioBlock<float> output_block);

    WorldSpaceNodeData world_space_node_data_;
    int read_position_ = 0;
};
