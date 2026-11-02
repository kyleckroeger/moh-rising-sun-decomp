/* A fragment of the sound driver (snddrv.c, 0x8016ada0): iSNDserve, the
   driver's service pass. For every packet player with a voice it counts down
   the start delay (then sets the voice's pitch, position and volume and
   starts each channel's AX voice as a 16-bit stream over its DSP buffer),
   and when playback passes the middle of the buffer into the other half it
   frees that half's frames and refills it; it then flushes the packet
   callbacks and frees the AX voices of software voices whose first AX voice
   has stopped. Then the voice range a render mode may use (mode bit 0x200:
   none below the first driver voice; bit 4: from the first driver voice to
   the voice count), resolving a timbre's sample data asynchronously (the
   data offset is taken from the header when none is given, and the sample
   pointer becomes the buffer plus the remaining offset), and
   SNDPLATFORM_playtimbre, which starts a timbre on a voice: through the mixer
   (each channel's mixer voice initialised with its sample data, length and
   loop, the voice's pitch, position, effect, time and filters set, then
   played) or, for DSP voices, through AX (ADPCM or 16-bit PCM addresses in
   nibbles or samples with the loop, or a silent loop, ADPCM coefficients and
   loop context per channel; -9 when no AX voice is free). sndgs, snddrv and
   the AX types are named by their symbols; the views of sndgs, snddrv, the
   AX voice, the packet player, the voice record and the patch header's
   members are inferred. The driver voice's active flag is volatile: the AX
   callback clears it. */
extern "C" {
void* memset(void*, int, unsigned long);
void SNDI_memcpy(void*, const void*, int);
}

/* inferred: the AX voice fields the service pass reads */
struct _AXVPB {
    unsigned char unknown000[326];
    unsigned short state;
    unsigned char unknown148[106];
    unsigned short currentHi;
    unsigned short currentLo;
};
typedef struct _AXVPB AXVPB;

struct AXPBADDR {
    unsigned short loopFlag;
    unsigned short format;
    unsigned short loopAddressHi;
    unsigned short loopAddressLo;
    unsigned short endAddressHi;
    unsigned short endAddressLo;
    unsigned short currentAddressHi;
    unsigned short currentAddressLo;
};

struct AXPBADPCM {
    unsigned short a[8][2];
    unsigned short gain;
    unsigned short pred_scale;
    unsigned short yn1;
    unsigned short yn2;
};

struct AXPBADPCMLOOP {
    unsigned short loop_pred_scale;
    unsigned short loop_yn1;
    unsigned short loop_yn2;
};

extern "C" {
void AXSetVoiceSrcType(AXVPB*, unsigned long);
void AXSetVoiceAdpcm(AXVPB*, AXPBADPCM*);
void AXSetVoiceAdpcmLoop(AXVPB*, AXPBADPCMLOOP*);
void AXSetVoiceAddr(AXVPB*, AXPBADDR*);
void AXSetVoiceState(AXVPB*, unsigned short);
}

/* inferred: the patch header's sample fields */
struct SNDIPATCHHEADER {
    unsigned char unknown00[3];
    signed char channels;
    unsigned char unknown04[15];
    unsigned char mixmode;
    unsigned char unknown14[66];
    unsigned short rate;
    unsigned int length;
    unsigned int loopStart;
    int loopEnd;
    unsigned char** data;
    int offset;
    int offsets[4];
    int extra[8];
    unsigned char* coefs[4];
    unsigned short* loopContext[4];
};

struct SNDVOICEVIEW {
    unsigned char unknown00[4];
    short voices[6];
    unsigned int length;
    unsigned char unknown14[8];
    unsigned short rate;
    unsigned char format;
    unsigned char channels;
    unsigned short flags;
    unsigned char unknown22[94];
};

struct SNDGSVIEW {
    unsigned char unknown000[51];
    unsigned char firstVoice;
    unsigned char unknown034[19];
    unsigned char packetCount;
    unsigned char unknown048[296];
    short voiceCount;
    unsigned char unknown172[98];
    SNDVOICEVIEW* voices;
};

struct SNDDRVVOICEVIEW {
    unsigned char unknown00[8];
    int offset;
    unsigned char unknown0c[8];
    volatile unsigned char active;
    unsigned char unknown15[23];
    AXVPB* ax;
};

/* inferred: a packet player's platform record */
struct SNDDRVPACKETVIEW {
    unsigned char buffer[4800];
    unsigned char* staging[3];
    signed char voice;
    unsigned char half;
    signed char delay;
    unsigned char unknown12cf;
    unsigned int dsp[3];
    unsigned int middle;
    short frames[2];
};

struct SNDDRVVIEW {
    SNDDRVVOICEVIEW voices[77];
    unsigned char unknown0e70[4];
    unsigned int silence;
    unsigned int silenceAdpcm;
    unsigned char unknown0e7c[8];
    SNDDRVPACKETVIEW* packets[1];
};

extern "C" SNDGSVIEW sndgs;
extern SNDDRVVIEW snddrv;

struct CHANPUB;
struct SNDDRVPKTCHAN;

void SNDPLATFORM_setpitch(int);
void SNDPLATFORM_set3dpos(int);
void SNDPLATFORM_setvol(int);
void SNDPKTPLAYI_freeframes(int, int, int);
void SNDPKTPLAYI_flushcallbackdata();
void SNDDRV_fillbufwithpackets(int, SNDDRVPKTCHAN*, CHANPUB*);
void SNDDRV_freeaxvoice(int, int);

void iSNDserve();

void SNDPLATFORM_getvoicerange(int mode, int* low, int* high);

int SNDPLATFORM_asyncresolvetimbre(SNDIPATCHHEADER* header, char* buffer, int* offset) {
    if (*offset == 0)
        *offset = header->offset;
    *header->data = (unsigned char*)(buffer + (header->offset - *offset));
    return 0;
}

int SNDDRV_allocateaxvoice(int);
void SNDPLATFORM_set3dpos(int);
void SNDPLATFORM_setvol(int);
void SNDPLATFORM_setpitch(int);
void SNDPLATFORM_setfxlevel(int, int);
void SNDPLATFORM_timemult(int, int);
void SNDPLATFORM_lowpass(int, int);
void SNDPLATFORM_highpass(int, int);
extern "C" {
void MIX_playinit(int, int, int, void*, int, int, int, unsigned int, unsigned int, int, int, int, int);
void MIX_play(int);
}

int SNDPLATFORM_playtimbre(SNDIPATCHHEADER* header, void* data, int index, int time, int unused, int lowpass, int highpass);
