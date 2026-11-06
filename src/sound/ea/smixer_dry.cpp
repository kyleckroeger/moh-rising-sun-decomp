// A fragment of the software mixer (smixer.c): SNDMIX_setdrygain
// stores a mixer channel's dry gain for an output and marks the channel's
// gains for update. sndmix is named by its symbol; its view and the channel
// records (96 bytes) are inferred. SNDMIX_setwetgain does the same for the
// wet gain.
#include "SoundMixerView.h"

extern "C" void SNDMIX_setdrygain(int channel, int output, float gain) {
    sndmix.channels[channel].dry[output] = gain;
    sndmix.channels[channel].dirty = 1;
}

extern "C" void SNDMIX_setwetgain(int channel, int output, float gain) {
    sndmix.channels[channel].wet[output] = gain;
    sndmix.channels[channel].dirty = 1;
}
