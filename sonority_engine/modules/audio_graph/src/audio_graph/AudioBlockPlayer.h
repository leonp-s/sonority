#pragma once
#include <juce_dsp/juce_dsp.h>

struct AudioBlockPlayerData
{
    const juce::dsp::AudioBlock<const float> audio_block_;
    int read_position_ = 0;
};

class AudioBlockPlayer
{
public:
    explicit AudioBlockPlayer () = default;
    void prepare (const juce::dsp::ProcessSpec & spec);
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing,
                  AudioBlockPlayerData & audio_block_player_data);
    void reset ();
};