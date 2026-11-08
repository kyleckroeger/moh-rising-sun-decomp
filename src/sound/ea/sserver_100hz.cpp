/* SNDSYSI_100hzserver (sserver.c): the sound library's periodic service. Advances the
   tick, runs the driver's service pass and the registered clients, then walks the
   playing voices: steps the pitch and volume modulation counters, the volume ramp
   (a ramp's end stops or clamps) and the volume envelope, whose segments are
   8-byte {duration, level} records. sndgs is named by its symbol; the views and
   the member names here are inferred from offsets. */
#include "SoundGlobals.h"

/* inferred: the members of a voice record the service touches */
struct SNDVOICESERVICEVIEW {
    int handle;
    char pad4[44];
    int ramprate;
    int ramptarget;
    int volume;
    int enveloperate;
    int envelope;
    int envelopeleft;
    char pad48[20];
    signed char segmentcount;
    signed char segment;
    char pad5e[7];
    unsigned char wheelperiod;
    unsigned char lfoperiod;
    unsigned char wheelcount;
    unsigned char lfocount;
    unsigned char state;
    char pad6a[2];
    int* segments;
    char pad70[8];
    void* wheel;
    void* lfo;
    char pad80[6];
    short lfopitch;
    char pad88[4];
};

void iSNDserve();
void iSNDcalcpitch(int);
void iSNDcalcvol(int);
int SNDPLATFORM_setpitch(int);
void SNDPLATFORM_setvol(int);
extern "C" void SNDstop(int);

void SNDSYSI_100hzserver() {
    int i;

    sndgs.tick++;
    iSNDserve();
    for (i = 0; i < sndgs.clientcount; i++)
        sndgs.clients[i]();
    for (i = 0; i < sndgs.voicecount; i++) {
        SNDVOICESERVICEVIEW* voices = (SNDVOICESERVICEVIEW*)sndgs.voices;
        SNDVOICESERVICEVIEW* voice = &voices[i];

        if (voice->state == 1 && voices[i].handle >= 0) {
            int changed;

            if (voice->lfo) {
                if (++voice->lfocount >= voice->lfoperiod)
                    voice->lfocount = 0;
                voice->lfopitch = 0;
                iSNDcalcpitch(i);
                SNDPLATFORM_setpitch(i);
            }
            changed = 0;
            if (voice->wheel) {
                changed = 1;
                if (++voice->wheelcount >= voice->wheelperiod)
                    voice->wheelcount = 0;
            }
            if (voice->ramprate) {
                changed = 1;
                voice->volume += voice->ramprate;
                if (voice->ramprate < 0) {
                    int target = voice->ramptarget;
                    if (voice->volume > target) goto ramped;
                    voice->volume = target;
                    voice->ramprate = 0;
                } else {
                    int target = voice->ramptarget;
                    if (voice->volume < target) goto ramped;
                    voice->volume = target;
                    voice->ramprate = 0;
                }
                ramped:;
                if (voice->volume < 0) {
                    SNDstop(voice->handle);
                    continue;
                }
            }
            voice->envelopeleft--;
            if (voice->enveloperate) {
                changed = 1;
                voice->envelope += voice->enveloperate;
            }
            if (voice->envelopeleft == 0) {
                if (++voice->segment >= voice->segmentcount) {
                    SNDstop(voice->handle);
                    continue;
                }
                int* segment = voice->segments + voice->segment * 2;
                int duration = segment[0];
                voice->envelopeleft = duration;
                if (duration < 0)
                    voice->envelopeleft = 0x7FFFFFFF;
                voice->enveloperate = ((segment[1] << 16) - voice->envelope) / voice->envelopeleft;
            }
            if (changed) {
                iSNDcalcvol(i);
                if (voice->handle >= 0)
                    SNDPLATFORM_setvol(i);
            }
        }
    }
}
