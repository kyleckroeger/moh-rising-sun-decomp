/* A fragment of the sound driver (snddrv.c): the packet
   player's platform overhead and creation (two 3 KB DSP buffers and three
   32-byte aligned staging areas in the caller's memory). snddrv is named by
   its symbol; its view and the packet player's platform record are
   inferred. SNDDRV_getmastervoice and SNDDRV_getsamplechan before it are not
   reconstructed (index addressing and a compare operand order differ). */
extern "C" {
void* memset(void*, int, unsigned long);
}
void* SNDPLATFORM_memalloc(int, int);

/* inferred: the packet player's platform record */
struct SNDDRVPACKETVIEW {
    unsigned char buffer[9600];
    unsigned char* staging[6];
    signed char state;
    unsigned char unknown2591[3];
    unsigned char* dsp[6];
    unsigned char unknown25f4[48];
};

struct SNDDRVVIEW {
    unsigned char unknown0000[5144];
    SNDDRVPACKETVIEW* packets[1];
};

extern SNDDRVVIEW snddrv;

int SNDPLATFORM_packetoverhead(void) {
    return sizeof(SNDDRVPACKETVIEW);
}

int SNDPLATFORM_packetplaycreate(int index, void* memory) {
    SNDDRVPACKETVIEW* packet = (SNDDRVPACKETVIEW*)memory;
    int i;

    memset(packet, 0, sizeof(SNDDRVPACKETVIEW));
    packet->dsp[0] = (unsigned char*)SNDPLATFORM_memalloc(0, 18432);
    if (!packet->dsp[0])
        return -9;
    for (i = 0; i < 6; i++) {
        packet->dsp[i] = packet->dsp[0] + i * 3072;
        packet->staging[i] = (unsigned char*)(((unsigned int)packet->buffer + i * 1600 + 31) & ~31);
    }
    packet->state = -1;
    snddrv.packets[index] = packet;
    return 0;
}
