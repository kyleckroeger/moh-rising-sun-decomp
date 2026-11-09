// RCMP::AV_PLAYER::SetSubtitle and the destructor: deletes the decoder, audio player and timer,
// destroys the streams (the audio one when separate) and frees the loaded files through the
// file system's hook and the stream buffers through RCMP's.
#include "AvPlayer.h"

struct FILESYSOPTSVIEW {
    int size;
    void* (*malloc)(const char*, int, int);
    int (*mfree)(void*);
    int unknown0c[2];
};

extern "C" {
void STREAM_destroy(int handle);
void FILESYS_getopts(FILESYSOPTSVIEW*);
}

static inline void FileFree(void* p) {
    FILESYSOPTSVIEW opts;

    opts.size = 20;
    FILESYS_getopts(&opts);
    opts.mfree(p);
}

namespace RCMP {

void AV_PLAYER::SetSubtitle(AV_SUBTITLE_ARRAY* subtitles) {
    m_subtitles = subtitles;
}

AV_PLAYER::~AV_PLAYER() {
    if (m_decoder) {
        delete m_decoder;
        m_decoder = 0;
    }
    if (m_audio) {
        delete m_audio;
        m_audio = 0;
    }
    if (m_timer) {
        delete m_timer;
        m_timer = 0;
    }
    STREAM_destroy(m_stream);
    m_stream = 0;
    if (m_separateAudio) {
        STREAM_destroy(m_audioStream);
        m_audioStream = 0;
    }
    if (m_videoData) {
        FileFree(m_videoData);
        m_videoData = 0;
    }
    if (m_audioData) {
        FileFree(m_audioData);
        m_audioData = 0;
    }
    if (m_videoBuffer) {
        rcmp_sys.release(m_videoBuffer);
        m_videoBuffer = 0;
    }
    if (m_audioBuffer) {
        rcmp_sys.release(m_audioBuffer);
        m_audioBuffer = 0;
    }
}

}
