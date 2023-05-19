#pragma once

#include <filesystem>
#include <hrtf/mysofa.h>
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

class SofaFilter
{
public:
    struct OpenOptions
    {
        std::filesystem::path hrtf_path;
        float sample_rate;
    };

    struct CartesianCoordinates
    {
        float x;
        float y;
        float z;
    };

    explicit SofaFilter (const OpenOptions & open_options);
    [[nodiscard]] int GetFilterLength () const;
    void GetFilterForCartesian (juce::dsp::AudioBlock<float> hrir_block,
                                float & left_delay,
                                float & right_delay,
                                CartesianCoordinates cartesian_coordinates);

    ~SofaFilter ();

private:
    int filter_length_ = 0;
    int err_ = 0;
    struct MYSOFA_EASY * hrtf_ = nullptr;
};
