#ifndef DRAWCONTEXT_H
#define DRAWCONTEXT_H
// A scoped view of the draw context. The two matrix pointers are named from how the sky box
// uses them.
#include "CMatrix.h"

enum ERenderBins { RenderBinUnknown = 0 };

class DrawCommand;
struct COORD3;

class CDrawContext {
public:
    unsigned char unknown000[192];
    CMatrix *modelView;
    CMatrix *camera;
    unsigned char unknown0c8[548 - 200];
    void AddCommand(ERenderBins bin, const DrawCommand &command);
    // The table pointer follows the fields, at +0x224. The table holds the destructor and four
    // pure virtual slots. Only the last slot's use is observed, by CStaticMesh::IsVisible, and its
    // name is inferred from that use.
    virtual ~CDrawContext();
    virtual void PureSlot1() = 0;
    virtual void PureSlot2() = 0;
    virtual void PureSlot3() = 0;
    virtual int IsSphereVisible(const COORD3 &center, float radius) = 0;
};

#endif
