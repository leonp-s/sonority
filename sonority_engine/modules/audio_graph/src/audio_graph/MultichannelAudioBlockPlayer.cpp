#include "MultichannelAudioBlockPlayer.h"

void MultichannelAudioBlockPlayer::Process (
    const juce::dsp::ProcessContextReplacing<float> & replacing,
    AudioBlockPlayerData & audio_block_player_data)
{
    auto & read_position = audio_block_player_data.read_position;
    auto audio_block = audio_block_player_data.audio_block;
    auto output_block = replacing.getOutputBlock ();
    auto output_block_length = output_block.getNumSamples ();
    auto audio_block_length = audio_block.getNumSamples ();


    for (auto channel_index = 0; channel_index < audio_block.getNumChannels (); ++channel_index)
    {
        auto current_audio_block = audio_block.getSubsetChannelBlock (channel_index, 1);
        auto current_output_block = output_block.getSubsetChannelBlock (channel_index, 1);

        if (read_position + output_block_length < audio_block_length)
        {
            auto write_block = current_audio_block.getSubBlock (read_position, output_block_length);
            current_output_block.copyFrom (write_block);
        }
        else
        {
            auto remaining_samples = audio_block_length - read_position;
            auto overflowing_samples = output_block_length - remaining_samples;

            auto remaining_block =
                current_audio_block.getSubBlock (read_position, remaining_samples);
            auto overflowing_block = current_audio_block.getSubBlock (0, overflowing_samples);

            current_output_block.copyFrom (remaining_block);
            current_output_block.getSubBlock (remaining_samples, overflowing_samples)
                .copyFrom (overflowing_block);
        }
    }

    output_block.multiplyBy (audio_block_player_data.volume);
    read_position = (read_position + output_block_length) % audio_block_length;
}