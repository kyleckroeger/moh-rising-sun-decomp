/* SNDPLATFORM_playtimbre (a fragment of the sound driver, snddrv.c): starts a timbre on a
   voice: through the mixer (each channel's mixer voice initialised with its sample data,
   length and loop, the voice's pitch, position, effect, time and filters set, then played)
   or, for DSP voices, through AX (ADPCM or 16-bit PCM addresses in nibbles or samples with
   the loop, or a silent loop, ADPCM coefficients and loop context per channel; -9 when no
   AX voice is free). sndgs, snddrv and SNDIPATCHHEADER are named by their symbols; the
   views and member names are inferred from offsets. */
#include "SoundGlobals.h"
struct _AXVPB;
typedef _AXVPB AXVPB;

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
void SNDI_memcpy(void*, const void*, int);
void* memset(void*, int, unsigned long);
void MIX_playinit(int, int, int, void*, void*, unsigned char*, int, int, int, int, int, int, int, int);
void MIX_play(int);
}

/* inferred: the patch header's sample fields */
struct SNDIPATCHHEADER {
    unsigned char unknown00[2];
    signed char field2;
    signed char channels;
    unsigned char unknown04[15];
    unsigned char mixmode;
    unsigned char unknown14[66];
    unsigned short rate;
    unsigned int length;
    unsigned int loopStart;
    int loopEnd;
    unsigned char** data;
    int offsets[6];
    int loopoffsets[6];
    unsigned char* stretch[6];
    int extra[6];
    unsigned char* coefs[6];
    unsigned short* loopContext[6];
};

/* inferred: the driver's per-channel record */
struct SNDDRVVOICEVIEW {
    char unknown0[24];
    int offset;
    char unknown28[8];
    volatile unsigned char allocated;
    char unknown25[23];
    AXVPB* axvoice;
};

struct SNDDRVVIEW {
    SNDDRVVOICEVIEW voices[80];
    char pad[8];
    unsigned int silencepcm;
    unsigned int silenceadpcm;
};

extern SNDDRVVIEW snddrv;

int SNDDRV_allocateaxvoice(int);
void SNDPLATFORM_set3dpos(int);
void SNDPLATFORM_setvol(int);
int SNDPLATFORM_setpitch(int);
int SNDPLATFORM_setfxlevel(int, int);
int SNDPLATFORM_timemult(int, int);
void SNDPLATFORM_lowpass(int, int);
void SNDPLATFORM_highpass(int, int);

int SNDPLATFORM_playtimbre(SNDIPATCHHEADER* header, void* data, int index, int time, int unused, int lowpass, int highpass) {
    SNDVOICEVIEW* voice;
    SNDDRVVOICEVIEW* drv;
    unsigned int current;
    unsigned int loopAddress;
    unsigned int endAddress;
    unsigned int step;
    unsigned int offset;
    unsigned int start;
    unsigned int end;
    int i;

    voice = &sndgs.voices[index];
    drv = &snddrv.voices[index];
    voice->rate = header->rate;
    voice->length = header->length;
    step = 0;
    offset = 0;
    voice->count = header->channels;
    if (voice->flags & 0x200) {
        AXPBADDR addr;
        AXPBADPCM adpcm;
        AXPBADPCMLOOP loop;

        memset(&loop, 0, sizeof(loop));
        if (header->loopEnd >= 0) {
            addr.loopFlag = 1;
            end = header->loopEnd;
            start = header->loopStart;
        } else {
            addr.loopFlag = 0;
            if (voice->type == 18)
                start = snddrv.silenceadpcm;
            else
                start = snddrv.silencepcm;
            end = header->length - 1;
        }
        if (voice->type == 18) {
            addr.format = 0;
            adpcm.gain = 0;
            adpcm.yn1 = 0;
            adpcm.yn2 = 0;
            step = ((header->length + 13) / 14 * 14 << 3) / 7;
            offset = (unsigned int)*header->data << 1;
            start = start * 8 / 7;
            end = end * 8 / 7;
        } else if (voice->type == 7) {
            addr.format = 10;
            adpcm.gain = 0x800;
            step = header->length;
            offset = (unsigned int)*header->data >> 1;
        }
        drv->offset = offset;
        if (!SNDDRV_allocateaxvoice(index))
            return -9;
        SNDPLATFORM_set3dpos(index);
        SNDPLATFORM_setvol(index);
        SNDPLATFORM_setpitch(index);
        SNDPLATFORM_setfxlevel(voice->channels[0], 0);
        for (i = 0; i < header->channels; i++) {
            if (voice->type == 18) {
                SNDI_memcpy(&adpcm, header->coefs[i], 32);
                adpcm.pred_scale = header->coefs[i][32];
                if (header->loopEnd > 0) {
                    loop.loop_pred_scale = *(unsigned char*)&header->loopContext[i][2];
                    loop.loop_yn1 = header->loopContext[i][0];
                    loop.loop_yn2 = header->loopContext[i][1];
                }
            }
            current = offset + step * i;
            loopAddress = header->loopEnd > 0 ? current + start : start;
            endAddress = current + end;
            endAddress -= 2;
            if (voice->type == 18) {
                current += 2;
                if ((loopAddress & 15) == 0)
                    loopAddress += 2;
                else if ((loopAddress & 15) == 1)
                    loopAddress += 1;
                if ((endAddress & 15) == 0)
                    endAddress += 2;
                else if ((endAddress & 15) == 1)
                    endAddress += 1;
            }
            AXSetVoiceSrcType(snddrv.voices[voice->channels[i]].axvoice, 1);
            AXSetVoiceAdpcm(snddrv.voices[voice->channels[i]].axvoice, &adpcm);
            AXSetVoiceAdpcmLoop(snddrv.voices[voice->channels[i]].axvoice, &loop);
            addr.endAddressHi = endAddress >> 16;
            addr.endAddressLo = endAddress;
            addr.currentAddressHi = current >> 16;
            addr.currentAddressLo = current;
            addr.loopAddressHi = loopAddress >> 16;
            addr.loopAddressLo = loopAddress;
            AXSetVoiceAddr(snddrv.voices[voice->channels[i]].axvoice, &addr);
            AXSetVoiceState(snddrv.voices[voice->channels[i]].axvoice, 1);
            snddrv.voices[voice->channels[i]].allocated = 1;
        }
    } else {
        for (i = 0; i < voice->count; i++)
            MIX_playinit(voice->channels[i] - sndgs.firstvoice, header->mixmode, 1, (char*)data + header->offsets[i], (char*)data + header->offsets[0] + header->loopoffsets[i], header->stretch[i], voice->count, voice->length, header->loopStart, header->loopEnd, 0, 0, header->field2, i);
        SNDPLATFORM_setpitch(voice->channels[0]);
        SNDPLATFORM_set3dpos(voice->channels[0]);
        SNDPLATFORM_setfxlevel(voice->channels[0], 0);
        SNDPLATFORM_timemult(voice->channels[0], time);
        SNDPLATFORM_lowpass(voice->channels[0], lowpass);
        SNDPLATFORM_highpass(voice->channels[0], highpass);
        for (i = 0; i < voice->count; i++)
            MIX_play(voice->channels[i] - sndgs.firstvoice);
    }
    return 0;
}
