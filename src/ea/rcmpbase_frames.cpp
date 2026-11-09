// RCMP::DECODER: codec release, frame and chunk access, and the chunk and codec-data constructors.
#include "Rcmp.h"

namespace RCMP {

void DECODER::FreeChosenCodec() {
    if (codec) {
        delete codec;
        codec = 0;
    }
}

unsigned int DECODER::GetCurrentFrameNumber() {
    if (!codec)
        return 0;
    return codec->GetCurrentFrameNumber();
}

float DECODER::GetFrameRate() {
    if (!codec)
        return 0.0f;
    return codec->GetFrameRate();
}

FRAME *DECODER::GetFrame(unsigned int frame) {
    if (!codec && !pending)
        idata.getChunk(this, idata.streamer, &pending);
    if (!codec)
        return 0;
    return codec->GetFrame(frame);
}

void DECODER::ReleaseFrame(FRAME *frame) {
    codec->ReleaseFrame(frame);
}

CHUNK *DECODER::GetChunk() {
    CHUNK *chunk;
    CHUNK *result = pending;
    if (!result) {
        idata.getChunk(this, idata.streamer, &chunk);
        result = chunk;
    } else {
        chunk = result;
        pending = 0;
    }
    return result;
}

void DECODER::ReleaseChunk(CHUNK *chunk) {
    idata.releaseChunk(this, idata.streamer, chunk);
}

CHUNK::CHUNK() {
    next = 0;
    size = 0;
    data = 0;
}

CODEC_IDATA::CODEC_IDATA() : streamer(0), getChunk(0), releaseChunk(0), value(2) {
}

CODEC_IDATA::CODEC_IDATA(STREAMER *s, GETCHUNKFUNC get, RELEASECHUNKFUNC release, unsigned int v) {
    value = v;
    streamer = s;
    getChunk = get;
    releaseChunk = release;
}

}
