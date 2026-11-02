// SNDstopall: stop every voice. sndgs's layout is not known; the voice table
// (140-byte entries starting with the handle) and voice count are read through
// inferred offsets.
extern "C" {
extern struct SNDGSVIEW sndgs;
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
int SNDstop(int);
}

struct SNDGSVIEW {
    char field0[382];
    short voicecount;
    char field180[112];
    char* voices;
};

extern "C" void SNDstopall(void) {
    int i;

    SNDSYS_entercritical();
    for (i = 0; i < sndgs.voicecount; i++)
        SNDstop(*(int*)(sndgs.voices + i * 140));
    SNDSYS_leavecritical();
}
