#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>

struct BufferWithSampleRate
{
    BufferWithSampleRate () = default;

    BufferWithSampleRate (juce::AudioBuffer<float> && bufferIn, double sampleRateIn)
        : buffer (std::move (bufferIn))
        , sampleRate (sampleRateIn)
    {
    }

    juce::AudioBuffer<float> buffer;
    double sampleRate = 0.0;
};

class BufferTransfer
{
public:
    void set (BufferWithSampleRate && p)
    {
        const juce::SpinLock::ScopedLockType lock (mutex);
        buffer = std::move (p);
        newBuffer = true;
    }

    template <typename Fn>
    void get (Fn && fn)
    {
        const juce::SpinLock::ScopedTryLockType lock (mutex);

        if (lock.isLocked () && newBuffer)
        {
            fn (buffer);
            newBuffer = false;
        }
    }

private:
    BufferWithSampleRate buffer;
    bool newBuffer = false;
    juce::SpinLock mutex;
};