#pragma once

#include <juce_core/juce_core.h>

class SofaRenderer
{
public:
    SofaRenderer();

    void SetFilter();
    void ProcessBlock();

private:
};