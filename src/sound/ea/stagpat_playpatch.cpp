// A fragment of stagpat.c: SNDBANKI_playpatch plays a patch: it takes a free patch key, parses
// the patch's timbres and starts every timbre whose volume and key ranges cover the play
// options, with one random detune for the whole patch. It then drops voices already over and
// tags the rest with the key, marking the last one; a lone voice needs no key. If a timbre
// fails or nothing is left, the started voices are stopped. The function and
// SNDIPATCHHEADER, SNDPLAYOPTS and TAGGEDPATCH are named by mangled symbols; the members are
// inferred.
struct TAGGEDPATCH;
struct SNDIPATCHHEADER {
    short detune;
    unsigned char unknown02[6];
    signed char lowVolume;
    signed char highVolume;
    signed char lowKey;
    signed char highKey;
    unsigned char unknown0c[244];
};
struct SNDPLAYOPTS {
    unsigned char unknown0[2];
    signed char key;
    signed char volume;
};
struct SNDVOICEVIEW {
    unsigned char unknown0[38];
    unsigned char patchKey;
    unsigned char last;
    unsigned char unknown28[100];
};
struct SNDGSVIEW {
    unsigned char unknown0[496];
    SNDVOICEVIEW* voices;
};
extern "C" SNDGSVIEW sndgs;
extern "C" int SNDover(int);
extern "C" int SNDstop(int);
int randrange(int);
int SNDVOICEI_get(int);
int SNDBANKI_findfreekey();
int SNDI_parsetimbre(void**, SNDIPATCHHEADER*);
void SNDBANKI_userdatacallback(SNDIPATCHHEADER*, int);
int SNDBANKI_playtimbre(int, int, void*, SNDPLAYOPTS*, SNDIPATCHHEADER*, int, int, int);

int SNDBANKI_playpatch(void* data, TAGGEDPATCH* patch, int bank, int index, SNDPLAYOPTS* opts) {
    SNDVOICEVIEW* voice = 0;
    int count = 0;
    int randomized = 0;
    int detune = 0;
    int result = -9;
    int i;
    int more;
    int total;
    SNDIPATCHHEADER header;
    int handles[12];
    if (!patch)
        return -8;
    if (!data)
        data = patch;
    int key = opts->key;
    int volume = opts->volume;
    patch = (TAGGEDPATCH*)((char*)patch + 4);
    int patchKey = SNDBANKI_findfreekey();
    do {
        more = SNDI_parsetimbre((void**)&patch, &header);
        if (!randomized) {
            if (header.detune)
                detune = randrange(header.detune);
            randomized = 1;
        }
        if (volume >= header.lowVolume && volume <= header.highVolume && key >= header.lowKey && key <= header.highKey) {
            result = SNDBANKI_playtimbre(bank, index, data, opts, &header, key, volume, detune);
            if (result < 0)
                goto fail;
            handles[count++] = result;
            SNDBANKI_userdatacallback(&header, 1);
        }
    } while (more);
    total = count;
    count = 0;
    for (i = 0; i < total; i++) {
        if (!SNDover(handles[i]))
            handles[count++] = handles[i];
    }
    if (!count)
        goto fail;
    if (count == 1) {
        int v = SNDVOICEI_get(handles[0]);
        if (v >= 0) {
            voice = &sndgs.voices[v];
            voice->patchKey = 0;
            voice->last = 0;
        }
    } else {
        for (i = 0; i < count; i++) {
            int v = SNDVOICEI_get(handles[i]);
            if (v >= 0) {
                voice = &sndgs.voices[v];
                voice->patchKey = patchKey;
                voice->last = 0;
            }
        }
        voice->last = 1;
    }
    return result;
fail:
    for (i = 0; i < count;)
        SNDstop(handles[i++]);
    return result;
}
