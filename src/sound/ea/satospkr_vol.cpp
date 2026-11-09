// A fragment of the azimuth to speaker level code (satospkr.c): SNDI_aztospkrvol reads each output's
// level for an azimuth from the speaker tables. sndgs and sndaztospkr are named by their symbols; the
// view of sndgs is inferred.
#include "SoundGlobals.h"

extern unsigned char* sndaztospkr[6];

void SNDI_aztospkrvol(int azimuth, float* levels) {
    int i;

    azimuth = azimuth >> 8 & 0xFF;
    for (i = 0; i < sndgs.channels; i++)
        levels[i] = (float)(int)sndaztospkr[i][azimuth] * (1.0f / 255.0f);
}
