#pragma once
#include "../AudioBlockPlayer.h"
#include "audio_engine/Vector3.h"

#include <juce_dsp/juce_dsp.h>

struct WorldSpaceNodeData
{
    Vector3 cartesian;
    AudioBlockPlayerData player_data;
    bool is_ambisonic;
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
