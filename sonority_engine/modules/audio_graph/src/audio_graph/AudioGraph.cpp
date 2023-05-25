#include "AudioGraph.h"

AudioGraph::AudioGraph ()
{
    world_space_nodes_.reserve (1000);
}

void AudioGraph::AddLoopingPlayer (juce::Uuid uuid,
                                   juce::dsp::AudioBlock<float> audio_block,
                                   bool is_ambisonic)
{
    world_space_nodes_.insert (
        {uuid,
         WorldSpaceNodeData {.player_data = AudioBlockPlayerData {.audio_block = audio_block},
                             .is_ambisonic = is_ambisonic}});
}

void AudioGraph::RemoveLoopingPlayer (juce::Uuid uuid)
{
    world_space_nodes_.erase (uuid);
}

void AudioGraph::UpdateLoopingPlayer (juce::Uuid uuid, Vector3 cartesian, float volume)
{
    if (! world_space_nodes_.contains (uuid))
        return;

    world_space_nodes_ [uuid].cartesian = cartesian;
    world_space_nodes_ [uuid].player_data.volume = volume;
}

void AudioGraph::prepare (const juce::dsp::ProcessSpec & spec)
{
    sofa_dodec_renderer_.prepare (spec);
    ambisonic_buffer_.setSize (9, spec.maximumBlockSize, false, false, false);
    temp_ambisonic_buffer.setSize (9, spec.maximumBlockSize);
}

void AudioGraph::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    auto output_block = replacing.getOutputBlock ();
    output_block.clear ();

    juce::dsp::AudioBlock<float> ambisonic_process_block {ambisonic_buffer_};
    ambisonic_process_block.clear ();

    juce::dsp::ProcessContextNonReplacing<float> ambisonic_context {output_block,
                                                                    ambisonic_process_block};

    for (auto & audio_block_player_data : world_space_nodes_)
    {
        if (audio_block_player_data.second.is_ambisonic)
        {
            juce::dsp::AudioBlock<float> temp_ambisonic_block {temp_ambisonic_buffer};
            temp_ambisonic_block.clear ();

            juce::dsp::ProcessContextReplacing<float> ambisonic_replacing {temp_ambisonic_block};
            AudioBlockPlayer::Process (ambisonic_replacing,
                                       audio_block_player_data.second.player_data);

            juce::dsp::ProcessContextNonReplacing<float> ambisonic_rotation_context {
                temp_ambisonic_buffer, ambisonic_process_block};
            ambisonic_rotator_.process (ambisonic_rotation_context,
                                        audio_block_player_data.second.cartesian);
        }
        else
        {
            AudioBlockPlayer::Process (replacing, audio_block_player_data.second.player_data);
            ambisonic_encoder_.process (ambisonic_context,
                                        audio_block_player_data.second.cartesian);
        }
    }

    output_block.clear ();
    juce::dsp::ProcessContextNonReplacing<float> sofa_context {ambisonic_process_block,
                                                               output_block};
    sofa_dodec_renderer_.process (sofa_context);
}

void AudioGraph::reset ()
{
    sofa_dodec_renderer_.reset ();
}