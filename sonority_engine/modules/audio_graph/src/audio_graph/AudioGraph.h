#pragma once
#include "AudioBlockPlayer.h"
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

    void AddLoopingPlayer (juce::Uuid uuid, juce::dsp::AudioBlock<float> audio_block);
    void RemoveLoopingPlayer (juce::Uuid uuid);
    void UpdateLoopingPlayer (juce::Uuid uuid, float volume);

private:
    SofaDodecRenderer sofa_dodec_renderer_;

    std::unordered_map<juce::Uuid, AudioBlockPlayerData> looping_data_;

    juce::AudioBuffer<float> ambisonic_buffer_;
    std::vector<int> schedule_;
};
