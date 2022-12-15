sonority_add_module(
    MODULE sonority_test_module
    SOURCES src/sonority_test_module/TestModuleClass.cpp
        src/sonority_test_module/TestModuleClass.h
    DEPS juce::juce_core
)

sonority_add_test(
    MODULE sonority_test_module
    SOURCES test/TestTestModuleClass.cpp
    DEPS sonority_test_module
)