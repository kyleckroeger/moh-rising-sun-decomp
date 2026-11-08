// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original staticmesh.cpp.
#include "StaticMesh.h"
extern "C" void *DWI_alloc(const char *, int, int);

void CStaticAnimMesh::Draw(CDrawContext &context, int a, unsigned int b, ERenderBins c, ERenderBins d) {
    SetFrame(a);
    CStaticMesh::Draw(context, a, b, c, d);
    SetFrame(0);
}

void CStaticAnimMesh::Init(int frameCount, int ticks) {
    ticksPerFrame = ticks;
    numFrames = frameCount;
    frames = (StaticMesh **)DWI_alloc("source/staticmesh/staticmesh.cpp:890", frameCount * 4, 0x400);
    for (int i = 0; i < numFrames; i++)
        frames[i] = 0;
}

void CStaticAnimMesh::Init(int frame, void *newMesh, bool load) {
    mesh = (StaticMesh *)newMesh;
    animShapes = 0;
    if (load)
        CStaticMesh::Init();
    else
        InitAnimatedTexturesOnly();
    frames[frame] = mesh;
    mesh = frames[0];
}

int CStaticAnimMesh::GetNumFrames() const {
    return numFrames;
}

int CStaticAnimMesh::GetTicksPerFrame() const {
    return ticksPerFrame;
}
