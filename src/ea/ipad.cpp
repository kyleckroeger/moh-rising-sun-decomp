// REAL pad driver, the last function of the file: iPAD_update copies the
// sampled vgPadStatus to gPadStatus under disabled interrupts, then probes
// each port (standard controller or WaveBird), marks newly connected
// controllers not ready and resets them, and marks other ports as having no
// controller. PADStatus is Dolphin's 12-byte pad status (as in pad.cpp). The
// sampling callback and iPAD_init come first in the file; iPAD_init is two
// lines off, so they are not part of this unit.
struct PADStatus {
    unsigned short button;
    signed char stickX;
    signed char stickY;
    signed char substickX;
    signed char substickY;
    unsigned char triggerLeft;
    unsigned char triggerRight;
    unsigned char analogA;
    unsigned char analogB;
    signed char err;
};

extern "C" {
int PADReset(unsigned long);
unsigned long SIProbe(int);
int OSDisableInterrupts(void);
int OSRestoreInterrupts(int);
void* MEM_copy(void*, const void*, int);
}

extern PADStatus vgPadStatus[4];
extern PADStatus gPadStatus[4];

void iPAD_update(void) {
    int i;
    unsigned long bit;
    unsigned long reset;
    int level;
    int type;

    level = OSDisableInterrupts();
    MEM_copy(gPadStatus, vgPadStatus, sizeof(gPadStatus));
    OSRestoreInterrupts(level);
    reset = 0;
    for (i = 0; i < 4; i++) {
        bit = 0x80000000 >> i;
        type = SIProbe(i);
        if (type == 0x09000000 || type == (int)0x8B100000) {
            if (gPadStatus[i].err == -1) {
                vgPadStatus[i].err = -2;
                reset |= bit;
            }
        } else {
            vgPadStatus[i].err = -1;
        }
    }
    if (reset)
        PADReset(reset);
}
