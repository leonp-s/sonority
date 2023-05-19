#include "SofaFilter.h"

SofaFilter::SofaFilter (const OpenOptions & open_options)
{
    hrtf_ = mysofa_open(open_options.hrtf_path.c_str(), open_options.sample_rate, &filter_length_, &err_);
    if(hrtf_==NULL)
        jassert (false);
}

SofaFilter::~SofaFilter ()
{
    mysofa_close (hrtf_);
}
