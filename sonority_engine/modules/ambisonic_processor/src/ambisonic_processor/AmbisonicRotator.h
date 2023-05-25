#pragma once
#include "audio_engine/Vector3.h"

#include <hrtf/mysofa.h>
#include <juce_dsp/juce_dsp.h>

class AmbisonicRotator
{
public:
    AmbisonicRotator ();
    void prepare (const juce::dsp::ProcessSpec & spec);
    void process (juce::dsp::ProcessContextNonReplacing<float> & processContext, Vector3 cartesian);

private:
    int temp_timer = 0;
    float azimuth_degrees = 0.f;

    void processWChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                          float azimuth);
    void processYChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                          float azimuth);
    void processZChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                          float azimuth);
    void processXChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                          float azimuth);
    void processVChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                          float azimuth);
    void processTChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                          float azimuth);
    void processRChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                          float azimuth);
    void processSChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                          float azimuth);
    void processUChannel (juce::dsp::ProcessContextNonReplacing<float> processContext,
                          float azimuth);

    juce::AudioBuffer<float> input_buffer_;
};
