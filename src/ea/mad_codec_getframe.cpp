// A fragment of EA's MAD movie codec (rcmp_mad_codec.cpp): stepping to a wanted frame. Names come from the symbol table;
// members and the chunk tags' meanings are inferred from the code. The destructor is declared first so this fragment
// does not own the table.
#include "Rcmp.h"

unsigned int GetChunkType(RCMP::CHUNK *); // A file-local function in the original, defined outside this fragment.

class MAD_CODEC_INTERNAL : public RCMP::CODEC {
  public:
    ~MAD_CODEC_INTERNAL();
    RCMP::FRAME *GetFrame(unsigned int);
    void GetNextChunk(RCMP::CHUNK **);
    void ReleaseChunk(RCMP::CHUNK *);
    RCMP::FRAME *DecodeChunk(RCMP::CHUNK *);
    unsigned char unknown04[0x0c - 4];
    int noSkipping;     // +0x0c
    unsigned char unknown10[0x18 - 0x10];
    RCMP::FRAME *held;  // +0x18
    unsigned int frame; // +0x1c
};

RCMP::FRAME *MAD_CODEC_INTERNAL::GetFrame(unsigned int wanted) {
    RCMP::CHUNK *chunk;
    GetNextChunk(&chunk);
    if (!noSkipping) {
        int skip = wanted - frame;
        if (skip > 2) {
            do {
                ReleaseChunk(chunk);
                GetNextChunk(&chunk);
            } while (GetChunkType(chunk) != 0x4d41446b);
        } else if (skip > 0) {
            for (int i = 0; i < skip; i++) {
                if (GetChunkType(chunk) == 0x4d414465) {
                    ReleaseChunk(chunk);
                    GetNextChunk(&chunk);
                }
            }
        }
    }
    RCMP::CHUNK *current = chunk;
    if (current) {
        RCMP::FRAME *result = DecodeChunk(current);
        ReleaseChunk(chunk);
        return result;
    }
    if (held) {
        ReleaseFrame(held);
        held = (RCMP::FRAME *)current;
    }
    return 0;
}
