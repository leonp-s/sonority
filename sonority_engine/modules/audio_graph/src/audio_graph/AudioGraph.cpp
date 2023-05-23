#include "AudioGraph.h"

AudioGraph::AudioGraph ()
{
    looping_data_.reserve (1000);
}

void AudioGraph::AddLoopingPlayer (juce::Uuid uuid, juce::dsp::AudioBlock<float> audio_block)
{
    looping_data_.insert ({uuid, AudioBlockPlayerData {.audio_block = audio_block}});
}

void AudioGraph::RemoveLoopingPlayer (juce::Uuid uuid)
{
    looping_data_.erase (uuid);
}

void AudioGraph::UpdateLoopingPlayer (juce::Uuid uuid, float volume)
{
    if (! looping_data_.contains (uuid))
        return;

    looping_data_ [uuid].volume = volume;
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

    //    for (world_space_node in worldspacenodes)
    //        ambisonic_renderer.RenderWorldNodeBlock(ambisonic_process_block, world_space_node);

    juce::dsp::ProcessContextNonReplacing<float> process_context {ambisonic_process_block,
                                                                  output_block};
    sofa_dodec_renderer_.process (process_context);

    for (auto & audio_block_player_data : looping_data_)
        AudioBlockPlayer::Process (replacing, audio_block_player_data.second);
}

void AudioGraph::reset ()
{
    sofa_dodec_renderer_.reset ();
}