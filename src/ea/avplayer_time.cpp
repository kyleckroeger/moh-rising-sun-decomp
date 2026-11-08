// RCMP::AV_PLAYER's timing: IsTimeForDecode compares the synced time in frames with the next
// frame, SetSpeed clamps and applies a speed to the audio player and timer, and SyncedAudioTime
// follows the audio stream's played position, moving the timer's offset by an eighth of the drift
// when it exceeds 264.
#include "AvPlayer.h"

extern "C" {
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
int SNDSTRM_status(int stream, int* status);
int SNDSTRM_requeststatus(int request, unsigned int* status);
int SNDPROFILE_outputlatency(void);
}

namespace RCMP {

bool AV_PLAYER::IsTimeForDecode() {
    unsigned int elapsed = SyncedAudioTime();
    float rate = m_decoder->GetFrameRate();

    m_time = (float)(elapsed + m_startTime) * 0.001f * rate;
    return m_time >= (float)m_frame + 1.0f;
}

int AV_PLAYER::SetSpeed(unsigned int speed) {
    if (speed > 0x4000)
        speed = 0x4000;
    if (m_audio)
        m_audio->SetSpeed(speed);
    return m_timer->SetSpeed(speed);
}

int AV_PLAYER::Pause() {
    return SetSpeed(0);
}

int AV_PLAYER::UnPause() {
    return SetSpeed(0x1000);
}

int AV_PLAYER::SetVol(unsigned int volume) {
    if (m_audio)
        return m_audio->SetVol(volume);
    return 0;
}

int AV_PLAYER::SyncedAudioTime() {
    unsigned int time = m_timer->GetTime();
    int elapsed = time - m_offset;

    if (m_audioSynced) {
        int request[4];
        int status[4];

        SNDSYS_entercritical();
        SNDSTRM_status(m_audio->stream, status);
        SNDSTRM_requeststatus(status[1], (unsigned int*)request);
        SNDSYS_leavecritical();
        int position = request[1] - SNDPROFILE_outputlatency();
        if (position > m_audioPosition) {
            int difference = position - elapsed;
            int drift = m_drift;
            m_audioPosition = position;
            m_drift = (drift - drift / 8) + difference;
            int magnitude = -m_drift;
            if (magnitude < m_drift)
                magnitude = m_drift;
            if (magnitude > 264) {
                m_offset -= m_drift / 8;
                m_drift = 0;
                elapsed = time - m_offset;
            }
        }
    }
    return elapsed;
}

}
