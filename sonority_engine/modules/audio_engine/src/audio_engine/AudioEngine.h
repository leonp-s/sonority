#pragma once
#include "AudioFilePool.h"
#include "audio_graph/AudioGraph.h"

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <optional>

struct Vector3
{
    float x;
    float y;
    float z;
};

struct VirtualSourceData
{
    bool is_playing;
    float volume;
    std::string file_path;
    Vector3 cartesian;
};

class VirtualSource
{
public:
    VirtualSource (AudioGraph & audio_graph, juce::Uuid source, AudioFilePool & audio_file_pool);
    void SourceDidUpdate (VirtualSourceData virtual_source_data);

private:
    juce::Uuid source_;
    AudioGraph & audio_graph_;
    VirtualSourceData virtual_source_data_;
    AudioFilePool & audio_file_pool_;

    AudioFilePool::SharedBufferHandle shared_buffer_handle_;

    void PlaySource (std::string file_path);
};

class AudioEngine
{
public:
    AudioEngine (AudioGraph & audio_graph);
    juce::Uuid RequestCreateSource ();
    void DeleteSource (juce::Uuid source);
    void SourceDidUpdate (juce::Uuid source, VirtualSourceData source_data);

private:
    AudioFilePool audio_file_pool_;
    AudioGraph & audio_graph_;
    std::unordered_map<juce::Uuid, VirtualSource> virtual_sources_;
};