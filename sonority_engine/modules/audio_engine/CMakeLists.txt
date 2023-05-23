sonority_add_module(
        MODULE audio_engine
        SOURCES src/audio_engine/AudioEngine.h src/audio_engine/AudioEngine.cpp src/audio_engine/AudioFilePool.h src/audio_engine/AudioFilePool.cpp
        DEPS juce::juce_core juce::juce_dsp audio_graph
)