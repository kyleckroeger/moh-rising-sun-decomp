// SNDI_calcfxlevel: a voice's send level to an effect bus (bus level times the
// voice's two per-bus levels). sndgs's layout is not known; the voice record
// (140 bytes) is viewed through inferred members.
extern "C" char sndgs[];

struct SNDVOICEVIEW {
    char field0[36];
    unsigned short fxbus;
    char field26[58];
    signed char send[1];
    signed char level[1];
    short fxlevel[21];
};

signed char* SNDCTRLI_getfxbus(int, int);

void SNDI_calcfxlevel(int bus, int voice) {
    SNDVOICEVIEW* entry = &(*(SNDVOICEVIEW**)(sndgs + 496))[voice];
    signed char* fx = SNDCTRLI_getfxbus(bus, entry->fxbus);

    entry->fxlevel[bus] = entry->send[bus] * entry->level[bus] * fx[2] >> 6;
}
