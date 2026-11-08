// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AnimObjFactory.cpp; member names are descriptive.
#include "PlacementNew.h"
#include "CMatrix.h"
#include "StaticMesh.h"
namespace EAGL {
class DynamicLoader {
  public:
    void Release();
};
}
struct AnimObjectFactoryBlock {
    EAGL::DynamicLoader **entries; // +0x00
    int value04;                   // +0x04
    int count;                     // +0x08
    int Size() const { return count; }
    EAGL::DynamicLoader *Back() const { unsigned int offset = count * 4 - 4; return *(EAGL::DynamicLoader **)((char *)entries + offset); }
    void Shrink() { count--; }
};
class CAnimObjectFactory {
  public:
    void AddDynamicLoaderToRelease(EAGL::DynamicLoader *);
    void ReleaseDynamicLoaders();
    unsigned char unknown_0[0x90];
    AnimObjectFactoryBlock block; // +0x90
};

void CAnimObjectFactory::AddDynamicLoaderToRelease(EAGL::DynamicLoader *loader) {
    block.entries[block.count++] = loader;
}

void CAnimObjectFactory::ReleaseDynamicLoaders() {
    while (block.Size()) {
        EAGL::DynamicLoader *l = block.Back();
        block.Shrink();
        l->Release();
    }
}

// A mesh slot: type 0 holds a CStaticMesh and bit 1 marks a CStaticAnimMesh; negative when empty.
class CAnyStaticMesh {
  public:
    void InitMesh(void *, bool);
    void InitAnim(int, int);
    void InitFrame(int, void *, bool);
    int numFrames() const;
    int ticksPerFrame() const;
    int type;
    unsigned char storage[40];
};

void CAnyStaticMesh::InitMesh(void *data, bool load) {
    type = 0;
    CStaticMesh *mesh = (CStaticMesh *)storage;
    if (mesh)
        new (mesh) CStaticMesh;
    mesh->Init(data, load);
}

void CAnyStaticMesh::InitAnim(int frameCount, int ticks) {
    type = 2;
    CStaticAnimMesh *mesh = (CStaticAnimMesh *)storage;
    if (mesh)
        new (mesh) CStaticAnimMesh;
    mesh->Init(frameCount, ticks);
}

void CAnyStaticMesh::InitFrame(int frame, void *data, bool load) { ((CStaticAnimMesh *)storage)->Init(frame, data, load); }

int CAnyStaticMesh::numFrames() const {
    if (type >= 0 && (type & 2))
        return ((const CStaticAnimMesh *)storage)->GetNumFrames();
    return 0;
}

int CAnyStaticMesh::ticksPerFrame() const {
    if (type >= 0 && (type & 2))
        return ((const CStaticAnimMesh *)storage)->GetTicksPerFrame();
    return 0;
}
