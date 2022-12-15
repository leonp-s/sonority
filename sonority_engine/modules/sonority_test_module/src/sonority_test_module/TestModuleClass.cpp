#include "TestModuleClass.h"
#include <juce_core/juce_core.h>

TestModuleClass::TestModuleClass ()
{
    print_me_ = juce::square(100);
}

int TestModuleClass::get_print_me ()
{
    return print_me_;
}
