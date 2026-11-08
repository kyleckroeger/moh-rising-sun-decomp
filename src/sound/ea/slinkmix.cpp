// Installs the main-CPU mixer entry points. The parameter types follow the
// definitions and the calls through these pointers; MIX_audioslice's is inferred
// from its caller.
struct SNDMIXOPTSVIEW;
extern "C" {
void MIX_create(SNDMIXOPTSVIEW*);
void MIX_destroy(void);
void MIX_audioslice(void**, int);
void MIX_playinit(int, int, int, void*, void*, unsigned char*, int, int, int, int, int, int, int, int);
void MIX_play(int);
void MIX_stop(int);
void MIX_setpitch(int, int);
extern void (*MIXinitfn)(SNDMIXOPTSVIEW*);
extern void (*MIXrestorefn)(void);
extern void (*MIXaudioslicefn)(void**, int);
extern void (*MIXplayinitfn)(int, int, int, void*, void*, unsigned char*, int, int, int, int, int, int, int, int);
extern void (*MIXplayfn)(int);
extern void (*MIXstopfn)(int);
extern void (*MIXsetpitchfn)(int, int);

void SNDSYS_linkmaincpumixer() {
    MIXinitfn = MIX_create;
    MIXrestorefn = MIX_destroy;
    MIXaudioslicefn = MIX_audioslice;
    MIXplayinitfn = MIX_playinit;
    MIXplayfn = MIX_play;
    MIXstopfn = MIX_stop;
    MIXsetpitchfn = MIX_setpitch;
}
}
