#pragma once
#include <juce_core/juce_core.h>

class TestModuleClass
{
public:
    TestModuleClass();

    int get_print_me();
private:
    int print_me_;
};