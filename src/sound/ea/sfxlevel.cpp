// SNDfxlevel: set a sound's send level to an effect bus on every platform
// voice. sndgs's layout is not known; the voice table is accessed through
// inferred offsets.
extern "C" char sndgs[];
int SNDVOICEI_get(int);
int iSNDpatchkey(int, int*);
void SNDI_calcfxlevel(int, int);
void SNDPLATFORM_setfxlevel(int, int);

extern "C" int SNDfxlevel(int handle, int bus, int level) {
    int voice = SNDVOICEI_get(handle);

    if (voice >= 0) {
        int key = -1;

        while (iSNDpatchkey(voice, &key)) {
            char* levels = (char*)(key * 140 + *(int*)(sndgs + 496)) + 97;

            levels[bus] = level;
            SNDI_calcfxlevel(bus, key);
            SNDPLATFORM_setfxlevel(key, bus);
        }
    }
    return voice;
}
