#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>

class AudioFilePool
{
public:
    using AudioBuffer = juce::AudioBuffer<float>;
    using SharedBufferHandle = std::shared_ptr<AudioBuffer>;
    AudioFilePool ();
    SharedBufferHandle GetOrLoadAudioFileFromPool (std::string file_path);

private:
    void ReadAudioFileToBuffer (std::string file_path, SharedBufferHandle shared_buffer_handle);
    std::map<std::string, std::weak_ptr<AudioBuffer>> audio_file_cache_;
};
