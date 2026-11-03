/* SNDPLATFORM_restore stops the driver (a fragment of snddrv.c): the platform
   hook, the AX auxiliary callback, the mixer, the ARAM manager, any held
   critical section, the thread and AX. sndgs and snddrv are named by their
   symbols; their members are inferred from offsets. */
#include "SoundGlobals.h"
struct OSThread;

extern "C" {
void AXQuit(void);
void AXRegisterAuxACallback(void (*)(void*, void*), void*);
void OSCancelThread(OSThread*);
void SNDSYS_leavecritical(void);
}
void SNDDRV_restoremixer();
void SNDARAM_restore(void);

/* inferred view of snddrv */
struct SNDDRVVIEW {
    char unknown0000[5136];
    void (*hook)(int);
    char unknown1414[6856 - 5140];
    int thread[198];
    char unknown2360[0xc048 - 6856 - 792];
    signed char mixerinited;
};

extern SNDDRVVIEW snddrv;

int SNDPLATFORM_restore() {
    if (snddrv.hook)
        snddrv.hook(4);
    AXRegisterAuxACallback(0, 0);
    if (snddrv.mixerinited)
        SNDDRV_restoremixer();
    SNDARAM_restore();
    while (sndgs.critical)
        SNDSYS_leavecritical();
    OSCancelThread((OSThread*)snddrv.thread);
    AXQuit();
    return 0;
}
