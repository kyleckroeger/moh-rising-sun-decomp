#ifndef AVPLAYER_H
#define AVPLAYER_H

// RCMP::AV_PLAYER, the movie player, and its timer. Class and method names come from the
// symbols; members are named from what the code does with them and are inferred.
#include "Rcmp.h"

typedef long long OSTime;

/* inferred: low memory as far as used here */
struct OSLOWMEM {
    unsigned char unknown00[248];
    unsigned int busclock;
};

extern "C" OSTime OSGetTime(void);

namespace RCMP {

class AV_MS_TIMER {
public:
    AV_MS_TIMER() {
        elapsed = 0;
        start = OSGetTime();
        speed = 0x1000;
    }
    static void operator delete(void* p);
    ~AV_MS_TIMER();
    void Update();
    unsigned int GetTime() {
        Update();
        return elapsed / (((OSLOWMEM*)0x80000000)->busclock / 4000);
    }
    int SetSpeed(unsigned int value) {
        OSTime now = OSGetTime();
        if (speed == 0x1000)
            elapsed += now - start;
        else
            elapsed += ((unsigned long long)speed * (now - start)) >> 12;
        start = now;
        speed = value;
        return 0;
    }

    OSTime start;
    unsigned long long elapsed;
    unsigned int speed;
};

inline void AV_MS_TIMER::operator delete(void* p) { rcmp_sys.release(p); }
inline AV_MS_TIMER::~AV_MS_TIMER() {}

class AV_SUBTITLE_ARRAY;

class AV_PLAYER;

class STREAMER {
public:
    STREAMER() { player = 0; }

    AV_PLAYER* player;
};

class AV_PLAYER {
public:
    enum LOAD_ENUM { LOAD_STREAM, LOAD_FILE, LOAD_MEMORY };
    enum SOUND_ENUM { SOUND_ON, SOUND_OFF };

    AV_PLAYER(const char*, int, const char*, int, LOAD_ENUM, SOUND_ENUM);
    ~AV_PLAYER();
    static void operator delete(void* p) { rcmp_sys.release(p); }
    void Init(const char*, int, int, const char*, int, int, LOAD_ENUM, SOUND_ENUM);
    void SetSubtitle(AV_SUBTITLE_ARRAY*);
    FRAME* GetFirstFrame(unsigned int, int);
    FRAME* GetFrame(float);
    bool IsTimeForDecode();
    int SetSpeed(unsigned int);
    int Pause();
    int UnPause();
    int SetVol(unsigned int);
    int SyncedAudioTime();
    void GetRCMPChunk(DECODER*, CHUNK**);
    static void StaticGetRCMPChunk(DECODER*, STREAMER*, CHUNK**);
    void ReleaseRCMPChunk(CHUNK*);
    static void StaticReleaseRCMPChunk(DECODER*, STREAMER*, CHUNK*);

    AUDIO_PLAYER* m_audio;
    void* m_videoBuffer;
    void* m_audioBuffer;
    const char* m_video;
    const char* m_audioName;
    int m_separateAudio;
    LOAD_ENUM m_load;
    SOUND_ENUM m_noAudio;
    char* m_videoData;
    int m_videoFile;
    char* m_audioData;
    int m_audioFile;
    int m_stream;
    int m_audioStream;
    int m_videoQueue;
    int m_audioQueue;
    int m_startTime;
    unsigned int m_frame;
    float m_time;
    int m_offset;
    int m_audioPosition;
    int m_audioSynced;
    int m_drift;
    AV_MS_TIMER* m_timer;
    STREAMER m_streamer;
    DECODER* m_decoder;
    int unknown68[2];
    AV_SUBTITLE_ARRAY* m_subtitles;
    FRAME* m_currentFrame;
};

}

#endif
