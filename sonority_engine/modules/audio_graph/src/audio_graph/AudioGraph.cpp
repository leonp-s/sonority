#include "AudioGraph.h"

AudioGraph::AudioGraph ()
{
    world_space_nodes_.reserve (1000);
}

void AudioGraph::AddLoopingPlayer (juce::Uuid uuid, juce::dsp::AudioBlock<float> audio_block)
{
    world_space_nodes_.insert (
        {uuid,
         WorldSpaceNodeData {.player_data = AudioBlockPlayerData {.audio_block = audio_block}}});
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
        AudioBlockPlayer::Process (replacing, audio_block_player_data.second.player_data);
        ambisonic_encoder_.process (ambisonic_context, audio_block_player_data.second.cartesian);
    }

    juce::dsp::ProcessContextNonReplacing<float> sofa_context {ambisonic_process_block,
                                                                  output_block};
    sofa_dodec_renderer_.process (sofa_context);
}

void AudioGraph::reset ()
{
    sofa_dodec_renderer_.reset ();
}