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
    float volume_;
    juce::dsp::AudioBlock<const float> audio_block_;
};

class WorldSpaceNode : public juce::dsp::ProcessorBase
{
public:
    explicit WorldSpaceNode (WorldSpaceNodeData world_space_node_data);
    ~WorldSpaceNode () override = default;

    void UpdateNodeData (WorldSpaceNodeData world_space_node_data);

    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

private:
    WorldSpaceNodeData world_space_node_data_;
    int read_position_ = 0;
};
