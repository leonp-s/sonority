#pragma once
#include "AudioBlockPlayer.h"

#include <juce_dsp/juce_dsp.h>

struct MultichannelAudioBlockPlayerData
{
    juce::dsp::AudioBlock<const float> audio_block;
    int read_position = 0;
    float volume;
};

class MultichannelAudioBlockPlayer
{
public:
    explicit MultichannelAudioBlockPlayer () = default;
    static void Process (const juce::dsp::ProcessContextReplacing<float> & replacing,
                         AudioBlockPlayerData & audio_block_player_data);
};