sonority_add_module(
        MODULE ambisonic_processor
        SOURCES
        src/ambisonic_processor/AmbisonicEncoder.cpp
        src/ambisonic_processor/AmbisonicEncoder.h
        DEPS juce::juce_core juce::juce_dsp
)