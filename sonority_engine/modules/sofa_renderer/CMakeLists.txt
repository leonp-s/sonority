sonority_add_module(
    MODULE sofa_renderer
    SOURCES src/sofa_renderer/SofaFilter.h
        src/sofa_renderer/SofaFilter.cpp
        src/sofa_renderer/SofaRenderer.h
        src/sofa_renderer/SofaRenderer.cpp
    DEPS juce::juce_core
)