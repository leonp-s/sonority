#include "SofaFilter.h"

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

void SofaFilter::GetFilterForCartesian (juce::dsp::AudioBlock<float> hrir_block,
                                        float & left_delay,
                                        float & right_delay,
                                        SofaFilter::CartesianCoordinates cartesian_coordinates)
{
    static constexpr int kLeftChannel = 0;
    static constexpr int kRightChannel = 1;

    mysofa_getfilter_float (hrtf_,
                            cartesian_coordinates.x,
                            cartesian_coordinates.y,
                            cartesian_coordinates.z,
                            hrir_block.getChannelPointer (kLeftChannel),
                            hrir_block.getChannelPointer (kRightChannel),
                            &left_delay,
                            &right_delay);
}
