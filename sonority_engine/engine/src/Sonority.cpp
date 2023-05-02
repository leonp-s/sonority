#include "Sonority.h"

static Sonority sonority;

Sonority::Sonority() {}

void Sonority::init() {
    // This creates audio...
    audio_device_manager_.initialiseWithDefaultDevices(0, 1);
    audio_device_manager_.playTestSound();
}

extern "C" {
    void Init() {
        sonority.init();
    }
}