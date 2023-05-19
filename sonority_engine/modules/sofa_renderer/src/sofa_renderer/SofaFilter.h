#pragma once

#include <hrtf/mysofa.h>
#include <juce_core/juce_core.h>
#include <filesystem>

class SofaFilter
{
public:
    struct OpenOptions {
        std::filesystem::path hrtf_path;
        float sample_rate;
    };

    explicit SofaFilter (const OpenOptions & open_options);


    ~SofaFilter ();
private:
    int filter_length_ = 0;
    int err_ = 0;
    struct MYSOFA_EASY *hrtf_ = nullptr;
};
