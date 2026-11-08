// RCMP::AUDIO_PLAYER: the movie player's audio, a tap-mode stream fed from a buffer
// allocated through the RCMP system hook.
#include "Rcmp.h"

struct SNDPLAYOPTS {
    unsigned char data[24];
};

extern "C" {
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
int SNDSYS_inited(void);
void DEBUG_break(void);
int SNDplaysetdef(SNDPLAYOPTS *opts);
int SNDSTRM_createtap(int arg0, SNDPLAYOPTS *opts, int arg2, int arg3, void *buffer, int size);
int SNDSTRM_overheadtap(int channels, int buffers);
int SNDSTRM_queuerequestid(int stream, int flags, int requestid);
int SNDSTRM_modifyhold(int request, int hold);
int SNDSTRM_pitchmult(int stream, int multiplier);
int SNDSTRM_vol(int stream, int volume);
int SNDSTRM_destroy(int index);
}

namespace RCMP {

AUDIO_PLAYER::AUDIO_PLAYER(int channels, int requestId) {
    if (!SNDSYS_inited()) {
        DEBUG_break();
        return;
    }
    buffer = 0;
    request = -1;
    stream = -1;
    SNDPLAYOPTS opts;
    SNDplaysetdef(&opts);
    int size = SNDSTRM_overheadtap(1, 30);
    buffer = rcmp_sys.allocate("AV::audiobuff", size, 0, 0, (int)rcmp_sys.field8);
    stream = SNDSTRM_createtap(channels, &opts, 1, 30, buffer, size);
    request = SNDSTRM_queuerequestid(stream, -1, requestId);
    speed = 1.0f;
}

AUDIO_PLAYER::~AUDIO_PLAYER() {
    if (buffer) {
        SNDSTRM_destroy(stream);
        rcmp_sys.release(buffer);
        buffer = 0;
    }
}

int AUDIO_PLAYER::SetSpeed(unsigned int value) {
    if (stream != -1) {
        SNDSYS_entercritical();
        int result = SNDSTRM_pitchmult(stream, value);
        SNDSYS_leavecritical();
        return result;
    }
    return 0;
}

int AUDIO_PLAYER::StartSound() {
    if (request != -1) {
        SNDSYS_entercritical();
        int result = SNDSTRM_modifyhold(request, 0);
        SNDSYS_leavecritical();
        return result;
    }
    return 0;
}

int AUDIO_PLAYER::SetVol(unsigned int value) {
    if (stream != -1) {
        SNDSYS_entercritical();
        int result = SNDSTRM_vol(stream, value);
        SNDSYS_leavecritical();
        return result;
    }
    return 0;
}

}
