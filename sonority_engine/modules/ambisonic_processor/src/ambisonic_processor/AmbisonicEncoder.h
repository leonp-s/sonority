#pragma once
#include <juce_dsp/juce_dsp.h>


class AmbisonicEncoder
{
public:

    void process(juce::dsp::ProcessContextNonReplacing<float> & processContext, float azimuth, float elevation);

private:
};

#endif // SONORITY_AMBISONICENCODER_H
