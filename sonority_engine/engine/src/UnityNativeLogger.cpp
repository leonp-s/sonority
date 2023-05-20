#include "UnityNativeLogger.h"

UnityNativeLogger::UnityNativeLogger (DebugCallbackFuncPtr debug_callback)
    : debug_callback_ (debug_callback)
{
}

void UnityNativeLogger::logMessage (const juce::String & message)
{
    debug_callback_ (message.toRawUTF8 ());
}
