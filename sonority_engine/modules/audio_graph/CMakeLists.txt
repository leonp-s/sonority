sonority_add_module(
        MODULE audio_graph
        SOURCES src/audio_graph/nodes/WorldSpaceNode.h src/audio_graph/nodes/WorldSpaceNode.cpp
        DEPS juce::juce_core juce::juce_dsp
)