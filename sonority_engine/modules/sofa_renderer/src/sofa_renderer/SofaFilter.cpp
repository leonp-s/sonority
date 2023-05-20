#include "SofaFilter.h"

//#define DEBUG_LOG_FMT(fmt, ...)            \
//    do                                     \
//    {                                      \
//        static char buf [4096];            \
//        sprintf (buf, fmt, ##__VA_ARGS__); \
//        DebugLog (buf);                    \
//    }                                      \
//    while (0)

SofaFilter::SofaFilter (const OpenOptions & open_options)
{
    hrtf_ = mysofa_open (
        open_options.hrtf_path.c_str (), open_options.sample_rate, &filter_length_, &err_);
    if (hrtf_ == NULL)
        jassert (false);
}

SofaFilter::~SofaFilter ()
{
    mysofa_close (hrtf_);
}

int SofaFilter::GetFilterLength () const
{
    return filter_length_;
}

void SofaFilter::GetFilterForSphericalCoordinates (
    juce::dsp::AudioBlock<float> hrir_block,
    float & left_delay,
    float & right_delay,
    SofaFilter::SphericalCoordinates spherical_coordinates)
{
    static constexpr int kLeftChannel = 0;
    static constexpr int kRightChannel = 1;

    auto coordinates = std::array<float, 3> {
        spherical_coordinates.azimuth_degrees, spherical_coordinates.elevation_degrees, 1.f};
    mysofa_s2c (coordinates.data ());

    //    DEBUG_LOG_FMT ();

    mysofa_getfilter_float (hrtf_,
                            coordinates [0],
                            coordinates [1],
                            coordinates [2],
                            hrir_block.getChannelPointer (kLeftChannel),
                            hrir_block.getChannelPointer (kRightChannel),
                            &left_delay,
                            &right_delay);
}
