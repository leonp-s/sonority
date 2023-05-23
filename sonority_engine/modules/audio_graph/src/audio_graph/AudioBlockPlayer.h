#pragma once
#include <juce_dsp/juce_dsp.h>

struct AudioBlockPlayerData
{
    juce::dsp::AudioBlock<const float> audio_block;
    int read_position = 0;
    float volume;
};

class AudioBlockPlayer
{
public:
    explicit AudioBlockPlayer () = default;
    static void Process (const juce::dsp::ProcessContextReplacing<float> & replacing,
                         AudioBlockPlayerData & audio_block_player_data);
};