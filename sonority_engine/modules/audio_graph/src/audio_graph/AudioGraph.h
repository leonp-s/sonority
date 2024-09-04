#pragma once

#include "AudioBlockPlayer.h"
#include "MultichannelAudioBlockPlayer.h"
#include "ambisonic_processor/AmbisonicEncoder.h"
#include "ambisonic_processor/AmbisonicRotator.h"
#include "audio_engine/Vector3.h"
#include "nodes/WorldSpaceNode.h"
#include "sofa_renderer/SofaDodecRenderer.h"

#include <juce_dsp/juce_dsp.h>

class AudioGraph : public juce::dsp::ProcessorBase
{
public:
    ~AudioGraph () override = default;
    AudioGraph ();

    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

    void
    AddLoopingPlayer (juce::Uuid uuid, juce::dsp::AudioBlock<float> audio_block, bool is_ambisonic);
    void RemoveLoopingPlayer (juce::Uuid uuid);
    void UpdateLoopingPlayer (juce::Uuid uuid, Vector3 cartesian, float volume);

private:
    SofaDodecRenderer sofa_dodec_renderer_;
    AmbisonicEncoder ambisonic_encoder_;
    AmbisonicRotator ambisonic_rotator_;
    std::unordered_map<juce::Uuid, WorldSpaceNodeData> world_space_nodes_;
    juce::AudioBuffer<float> ambisonic_buffer_;
    juce::AudioBuffer<float> player_buffer_;
    juce::AudioBuffer<float> temp_ambisonic_buffer;
    std::vector<int> schedule_;
};
