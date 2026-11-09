// RCMP::DECODER: its constructor and destructor, which holds the class table.
#include "Rcmp.h"

namespace RCMP {

DECODER::DECODER(const CODEC_IDATA *data) {
    idata = *data;
    codec = 0;
    pending = 0;
}

DECODER::~DECODER() {
    FreeChosenCodec();
}

}
