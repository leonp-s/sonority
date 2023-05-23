#include "AudioEngine.h"

VirtualSource::VirtualSource (AudioGraph & audio_graph,
                              juce::Uuid source,
                              AudioFilePool & audio_file_pool)
    : audio_graph_ (audio_graph)
    , source_ (source)
    , audio_file_pool_ (audio_file_pool)
{
}

void VirtualSource::SourceDidUpdate (VirtualSourceData virtual_source_data)
{
    if (virtual_source_data_.is_playing != virtual_source_data.is_playing)
    {
        if (virtual_source_data.is_playing)
            PlaySource (virtual_source_data.file_path);
        else
            audio_graph_.RemoveLoopingPlayer (source_);
    }

    if (virtual_source_data.is_playing)
        audio_graph_.UpdateLoopingPlayer (source_, virtual_source_data.volume);

    virtual_source_data_ = virtual_source_data;
}

void VirtualSource::PlaySource (std::string file_path)
{
    juce::AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats ();

    shared_buffer_handle_ = audio_file_pool_.GetOrLoadAudioFileFromPool (file_path);
    audio_graph_.AddLoopingPlayer (source_, juce::dsp::AudioBlock<float> (*shared_buffer_handle_));
}

AudioEngine::AudioEngine (AudioGraph & audio_graph)
    : audio_graph_ (audio_graph)
{
}

juce::Uuid AudioEngine::RequestCreateSource ()
{
    auto uuid = juce::Uuid ();
    virtual_sources_.insert ({uuid, VirtualSource (audio_graph_, uuid, audio_file_pool_)});
    return uuid;
}

void AudioEngine::DeleteSource (juce::Uuid source)
{
    audio_graph_.RemoveLoopingPlayer (source);
}

void AudioEngine::SourceDidUpdate (juce::Uuid source, VirtualSourceData source_data)
{
    auto & virtual_source = virtual_sources_.at (source);
    virtual_source.SourceDidUpdate (source_data);
}
