// REAL pad interface on the platform layer. gPadStatus holds one 12-byte
// status record per controller (the size of Dolphin's PADStatus).
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

extern PADStatus gPadStatus[];
void iPAD_init(void);
void iPAD_restore(void);
void iPAD_update(void);

extern "C" {
void PAD_init(void) {
    iPAD_init();
}

void PAD_restore(void) {
    iPAD_restore();
}

PADStatus* PAD_getdataptr(int pad) {
    return &gPadStatus[pad];
}

int PAD_active(int pad) {
    return !gPadStatus[pad].err;
}

int PAD_state(int pad) {
    if (!gPadStatus[pad].err)
        return gPadStatus[pad].button;
    return 0;
}

int PAD_scan(int mask) {
    int i;

    for (i = 0; i < 4; i++) {
        if (PAD_active(i) && (PAD_state(i) & mask))
            return 1;
    }
    return 0;
}

void PAD_update(void) {
    iPAD_update();
}
}
