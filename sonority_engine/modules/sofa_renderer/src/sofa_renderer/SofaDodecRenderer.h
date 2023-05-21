#pragma once
#include "SofaFilter.h"
#include "SofaRenderer.h"

#include <juce_core/juce_core.h>

class SofaDodecRenderer
{
public:
    SofaDodecRenderer ();
    ~SofaDodecRenderer ();

    void prepare (const juce::dsp::ProcessSpec & spec);
    void process (const juce::dsp::ProcessContextNonReplacing<float> & processContext);
    void reset ();

private:
    float sample_rate_;

    std::array<SofaRenderer, 12> sofa_renderers_;
    SofaFilter sofa_filter_ {SofaFilter::OpenOptions {
        .hrtf_path = std::filesystem::path (
            "/Users/micahstrange/sonority/sonority_engine/RIEC_hrir_subject_001.sofa"),
        .sample_rate = 48000,
    }};
    std::array<juce::AudioBuffer<float>, 12> hrir_buffers_;
    std::array<float, 12> left_delays_;
    std::array<float, 12> right_delays_;

    juce::AudioBuffer<float> renderer_input_buffer_;
    juce::AudioBuffer<float> renderer_output_buffer_;

    static constexpr std::array<SofaFilter::SphericalCoordinates, 12> kSphericalCoordinates {
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 180.f, .elevation_degrees = 63.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 50.f, .elevation_degrees = 46.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 310.f, .elevation_degrees = 46.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 118.f, .elevation_degrees = 16.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 242.f, .elevation_degrees = 16.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 0.f, .elevation_degrees = 0.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 180.f, .elevation_degrees = 0.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 62.f, .elevation_degrees = -16.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 298.f, .elevation_degrees = -16.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 130.f, .elevation_degrees = -46.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 230.f, .elevation_degrees = -46.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 0.f, .elevation_degrees = -63.f},
    };

    using DodecGain = std::array<float, 9>;
    using DodecGainTable = std::array<DodecGain, 12>;
    // clang-format off
    static constexpr DodecGainTable kGainTable {
        DodecGain {0.152978f,  0.000000f,  0.317213f, -0.157821f, -0.000000f,  0.000000f,  0.214016f, -0.209331f,  0.052905f},
        DodecGain {0.154588f,  0.187476f,  0.257668f,  0.158307f,  0.121955f,  0.199670f,  0.088563f,  0.170418f, -0.020269f},
        DodecGain {0.154588f, -0.187476f,  0.257668f,  0.158307f, -0.121955f, -0.199670f,  0.088563f,  0.170418f, -0.020269f},
        DodecGain {0.150438f,  0.299018f,  0.097531f, -0.158059f, -0.199547f,  0.123865f, -0.119296f, -0.066991f, -0.138360f},
        DodecGain {0.150438f, -0.299018f,  0.097531f, -0.158059f,  0.199547f, -0.123865f, -0.119296f, -0.066991f, -0.138360f},
        DodecGain {0.149842f, -0.000000f,  0.001933f,  0.351424f, -0.000000f,  0.000000f, -0.152550f,  0.002476f,  0.264353f},
        DodecGain {0.149842f, -0.000000f, -0.001933f, -0.351424f, -0.000000f, -0.000000f, -0.152550f,  0.002476f,  0.264353f},
        DodecGain {0.150438f,  0.299018f, -0.097531f,  0.158059f,  0.199547f, -0.123865f, -0.119296f, -0.066991f, -0.138360f},
        DodecGain {0.150438f, -0.299018f, -0.097531f,  0.158059f, -0.199547f,  0.123865f, -0.119296f, -0.066991f, -0.138360f},
        DodecGain {0.154588f,  0.187476f, -0.257668f, -0.158307f, -0.121955f, -0.199670f,  0.088563f,  0.170418f, -0.020269f},
        DodecGain {0.154588f, -0.187476f, -0.257668f, -0.158307f,  0.121955f,  0.199670f,  0.088563f,  0.170418f, -0.020269f},
        DodecGain {0.152978f,  0.000000f, -0.317213f,  0.157821f, -0.000000f,  0.000000f,  0.214016f, -0.209331f,  0.052905f}
    };
    // clang-format on
};
