#pragma once
#include <juce_core/juce_core.h>

typedef void (*DebugCallbackFuncPtr) (const char *);

class UnityNativeLogger : public juce::Logger
{
public:
    explicit UnityNativeLogger (DebugCallbackFuncPtr debug_callback);
    ~UnityNativeLogger () override = default;

protected:
    DebugCallbackFuncPtr debug_callback_;
    void logMessage (const juce::String & message) override;
};
