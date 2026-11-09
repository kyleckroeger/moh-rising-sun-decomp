// GCHW_VD: showing a frame, redrawing the current one and the empty completion hook.
#include "VideoDriver.h"

void GCHW_VD::Draw(RCMP::FRAME *newFrame) {
    if (frame)
        decoder->ReleaseFrame(frame);
    frame = newFrame;
    iDraw();
}

void GCHW_VD::ReDraw() {
    iDraw();
}

void GCHW_VD::CompleteDraw() {
}
