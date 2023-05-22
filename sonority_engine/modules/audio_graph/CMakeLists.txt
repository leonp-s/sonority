sonority_add_module(
        MODULE audio_graph
        SOURCES src/audio_graph/nodes/WorldSpaceNode.h
        src/audio_graph/nodes/WorldSpaceNode.cpp
        src/audio_graph/AudioBlockPlayer.h
        src/audio_graph/AudioBlockPlayer.cpp
        src/audio_graph/AudioGraph.h
        src/audio_graph/AudioGraph.cpp
        DEPS juce::juce_core juce::juce_dsp sofa_renderer
)