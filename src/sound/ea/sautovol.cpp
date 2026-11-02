// SNDautovol: fade a sound's volume over `time` updates on every platform
// voice. sndgs's layout is not known; the voice table is accessed through
// inferred offsets (16.16 fixed-point volumes).
#include "SoundGlobals.h"
int SNDVOICEI_get(int);
int iSNDpatchkey(int, int*);

extern "C" int SNDautovol(int handle, int time, int volume) {
    int voice = SNDVOICEI_get(handle);

    if (voice >= 0) {
        int key;

        if (time <= 0)
            time = 1;
        key = -1;
        while (iSNDpatchkey(voice, &key)) {
            int* entry = (int*)((char*)sndgs.voices + key * 140);

            entry[13] = volume << 16;
            entry[12] = (entry[13] - entry[14]) / time;
        }
    }
    return voice;
}
