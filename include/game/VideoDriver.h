#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H
// RCMP's video driver and its hardware implementation. Class and method names come from the
// original symbols; member names follow what the code does with them.
#include "Rcmp.h"

struct tBigYUVSwizzler;

namespace RCMP {

class VIDEO_DRIVER {
public:
    virtual ~VIDEO_DRIVER() {}
    virtual void SetPosition(float, float, float) = 0;
    virtual void SetScale(float, float) = 0;
    virtual void Draw(FRAME *) = 0;
    virtual void ReDraw() = 0;
    virtual void CompleteDraw() {}

    DECODER *decoder;
};

}

class GCHW_VD : public RCMP::VIDEO_DRIVER {
public:
    virtual ~GCHW_VD();
    static void operator delete(void *p) { RCMP::rcmp_sys.release(p); }
    virtual void SetPosition(float x, float y, float z);
    virtual void SetScale(float x, float y);
    virtual void Draw(RCMP::FRAME *frame);
    virtual void ReDraw();
    virtual void CompleteDraw();
    void iDraw();

    tBigYUVSwizzler *swizzler;
    unsigned char textures[96];
    RCMP::FRAME *frame;
    float width;
    float height;
    float z;
    float x;
    float y;
    float right;
    float bottom;
    float scaleX;
    float scaleY;
};

#endif
