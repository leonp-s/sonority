#include "AudioGraph.h"

AudioGraph::AudioGraph ()
{
    juce::AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats ();

    std::unique_ptr<juce::AudioFormatReader> reader (audioFormatManager.createReaderFor (
        juce::File ("/Users/LeonPS/Documents/Development/sonority/sonority_engine/vocdemo.wav")));
    fileBuffer_.setSize (reader->numChannels, reader->lengthInSamples);
    reader->read (&fileBuffer_, 0, reader->lengthInSamples, 0, true, true);
    //
    //    world_space_node_ = std::make_unique<WorldSpaceNode> (
    //        WorldSpaceNodeData {.volume_ = 1.f,
    //                            .is_looping_ = true,
    //                            .coordinates_ = Vector3 {.x = 1.f, .y = 2.f, .z = 3.f},
    //                            .audio_block_ = juce::dsp::AudioBlock<float> (fileBuffer_)});

    looping_data_.reserve (1000);

    AddLoopingPlayer (juce::Uuid (), juce::dsp::AudioBlock<float> (fileBuffer_));
}

void AudioGraph::AddLoopingPlayer (juce::Uuid uuid, juce::dsp::AudioBlock<float> audio_block)
{
    looping_data_.insert ({uuid, AudioBlockPlayerData {.audio_block_ = audio_block}});
}

void AudioGraph::RemoveLoopingPlayer (juce::Uuid uuid)
{
}

void AudioGraph::UpdateLoopingPlayer (juce::Uuid uuid, float volume)
{
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
        audio_block_player_.process (replacing, audio_block_player_data.second);
}

void AudioGraph::reset ()
{
    sofa_dodec_renderer_.reset ();
}