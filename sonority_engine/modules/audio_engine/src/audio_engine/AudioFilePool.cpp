#include "AudioFilePool.h"

AudioFilePool::AudioFilePool ()
{
}

AudioFilePool::SharedBufferHandle AudioFilePool::GetOrLoadAudioFileFromPool (std::string file_path)
{
    auto & cached_audio_buffer_wp = audio_file_cache_ [file_path];

    std::shared_ptr<AudioBuffer> sp;

    if (cached_audio_buffer_wp.expired ())
    {
        sp = std::make_shared<AudioBuffer> ();
        ReadAudioFileToBuffer (file_path, sp);
        cached_audio_buffer_wp = sp;
    }

    return cached_audio_buffer_wp.lock ();
}

void AudioFilePool::ReadAudioFileToBuffer (std::string file_path,
                                           AudioFilePool::SharedBufferHandle shared_buffer_handle)
{
    juce::AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats ();

    std::unique_ptr<juce::AudioFormatReader> reader (
        audioFormatManager.createReaderFor (juce::File (file_path)));

    shared_buffer_handle->setSize (reader->numChannels, reader->lengthInSamples);
    reader->read (shared_buffer_handle.get (), 0, reader->lengthInSamples, 0, true, true);
}
