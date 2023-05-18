sonority_add_module(
        MODULE audio_engine
        SOURCES src/audio_engine/CommandQueue.h src/audio_engine/CommandQueue.cpp
        DEPS juce::juce_core
)

#sonority_add_test(
#        MODULE sonority_test_module
#        SOURCES test/TestTestModuleClass.cpp
#        DEPS sonority_test_module
#)