#ifndef DRAWCONTEXT_H
#define DRAWCONTEXT_H
// A scoped view of the draw context. The two matrix pointers are named from how the sky box
// uses them.
#include "CMatrix.h"

enum ERenderBins { RenderBinUnknown = 0 };

class DrawCommand;

class CDrawContext {
public:
    unsigned char unknown000[192];
    CMatrix *modelView;
    CMatrix *camera;
    unsigned char unknown0c8[548 - 200];
    void AddCommand(ERenderBins bin, const DrawCommand &command);
};

#endif
