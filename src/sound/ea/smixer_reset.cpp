// A fragment of the software mixer (smixer.c): MIX_reset installs the default
// mixing and 16-bit decoding routines and the default frame length (320
// samples). sndmix is named by its symbol; its view is inferred.
#include "SoundMixerView.h"

extern "C" {
void mixc(void);
void decode16x87(void);
void MIX_reset(void) {
    sndmix.mix = mixc;
    sndmix.decode = decode16x87;
    sndmix.framelength = 320;
}
}
