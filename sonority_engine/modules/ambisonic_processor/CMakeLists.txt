sonority_add_module(
        MODULE ambisonic_processor
        SOURCES
        src/ambisonic_processor/AmbisonicEncoder.cpp
        src/ambisonic_processor/AmbisonicEncoder.h
        src/ambisonic_processor/AmbisonicRotator.cpp
        src/ambisonic_processor/AmbisonicRotator.h
        DEPS juce::juce_core juce::juce_dsp audio_engine
)