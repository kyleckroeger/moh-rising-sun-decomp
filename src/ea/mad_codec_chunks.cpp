// A fragment of EA's MAD movie codec (rcmp_mad_codec.cpp): the codec's chunk stepping.
// Names come from the symbol table; members are inferred from the code. The class is virtual in the original; this view
// does not need the table.
#include "Rcmp.h"
class MAD_CODEC_INTERNAL {
  public:
    int Init(RCMP::DECODER *, RCMP::CHUNK *);
    unsigned int GetCurrentFrameNumber();
    float GetFrameRate();
    void GetNextChunk(RCMP::CHUNK **);
    void ReleaseChunk(RCMP::CHUNK *);
    unsigned char unknown00[4];
    float frameRate; // +0x04
    unsigned char unknown08[0x18 - 8];
    RCMP::FRAME *held;   // +0x18
    unsigned int frame;  // +0x1c
    unsigned char unknown20[0x30 - 0x20];
    RCMP::DECODER *decoder; // +0x30
};

int MAD_CODEC_INTERNAL::Init(RCMP::DECODER *newDecoder, RCMP::CHUNK *) {
    frame = 0;
    decoder = newDecoder;
    held = 0;
    return 2;
}

unsigned int MAD_CODEC_INTERNAL::GetCurrentFrameNumber() { return frame; }

float MAD_CODEC_INTERNAL::GetFrameRate() { return frameRate; }

void MAD_CODEC_INTERNAL::GetNextChunk(RCMP::CHUNK **chunk) {
    *chunk = decoder->GetChunk();
    frame++;
}

void MAD_CODEC_INTERNAL::ReleaseChunk(RCMP::CHUNK *chunk) {
    if (chunk)
        decoder->ReleaseChunk(chunk);
}
