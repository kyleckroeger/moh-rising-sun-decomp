/* sclcptch.c: iSNDdetunetolinear converts a detune in cents to a linear pitch
   factor (4096 at no detune: whole octaves double or halve it, the remaining
   cents index the sndcents table). iSNDcalcpitch, after it, is not
   reconstructed yet. sndcents is named by its symbol. */
#include "SoundGlobals.h"
extern "C" unsigned char sndcents[];

int iSNDdetunetolinear(int detune) {
    int scale = 4096;
    int index;

    while (detune >= 1200) {
        scale <<= 1;
        detune -= 1200;
    }
    while (detune <= -1200) {
        scale >>= 1;
        detune += 1200;
    }
    index = detune * 13981 >> 16;
    if (index < -255)
        index = -255;
    if (index < 0)
        scale = scale * (sndcents[index + 256] + 256) >> 9;
    else
        scale = scale * (sndcents[index] + 256) >> 8;
    return scale;
}


int iSNDdetunetolinear(int);

void iSNDcalcpitch(int voice);
