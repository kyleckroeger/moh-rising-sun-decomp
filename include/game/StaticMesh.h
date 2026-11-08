#ifndef GAME_STATICMESH_H
#define GAME_STATICMESH_H
#include "CVector3.h"
#include "DrawContext.h"

// Class, method and static member names come from the symbol table. Member names are inferred from
// the reconstructed methods. Virtual functions the original defines inline after the file's
// static initialization are declared here without bodies.
class AnimShape;
class MaterialAnim;

// A mesh part's bounding data within the loaded file.
struct StaticMeshPart {
    unsigned char unknown00[108];
    CVector3 boxMin;     // +0x6c, three floats used
    CVector3 boxMax;     // +0x7c, three floats used
    float center[3];     // +0x8c
    float radius;        // +0x98
};

struct MaterialAnimList {
    int count;
    MaterialAnim *anims[1];
};

struct StaticMesh {
    unsigned char unknown00[32];
    StaticMeshPart *part0;          // +0x20
    StaticMeshPart *part1;          // +0x24
    unsigned char unknown28[64 - 40];
    MaterialAnimList *materialAnims; // +0x40
};

class CStaticMesh {
public:
    CStaticMesh();
    CStaticMesh(StaticMesh *mesh, bool load);
    void Init();
    void Init(void *mesh, bool load);
    void InitAnimatedTexturesOnly();
    void RebindAnimatedTextures();
    void GetCenter(CVector3 &center) const;
    void GetBasis(CVector3 &right, CVector3 &up, CVector3 &front) const;
    virtual ~CStaticMesh();
    virtual void Update(float elapsed);
    virtual void SetFrame(int frame);
    virtual void Draw(CDrawContext &context, int, unsigned int, ERenderBins, ERenderBins);
    virtual void EnableLighting(bool enable);
    virtual void EnableFogging(bool enable);
    virtual bool IsLightingEnabled();
    virtual bool IsFoggingEnabled();
    virtual unsigned int IsDrawEnabled() const;
    virtual int IsVisible(CDrawContext &context) const;

    StaticMesh *mesh;   // +0x00
    bool lighting;      // +0x04
    bool fogging;       // +0x08
    struct AnimShapeList *animShapes; // +0x0c
    int unknown10;      // +0x10
    int unknown14;      // +0x14
};

class CStaticAnimMesh : public CStaticMesh {
public:
    CStaticAnimMesh() : frames(0), numFrames(0) {}
    void Init(int frameCount, int ticks);
    void Init(int frame, void *mesh, bool load);
    int GetNumFrames() const;
    int GetTicksPerFrame() const;
    virtual ~CStaticAnimMesh();
    virtual void SetFrame(int frame);
    virtual void Draw(CDrawContext &context, int, unsigned int, ERenderBins, ERenderBins);

    StaticMesh **frames; // +0x1c
    int numFrames;       // +0x20
    int ticksPerFrame;   // +0x24
};
// Defined after CStaticAnimMesh: that class's inline constructor calls this one out of line in the
// original, while direct construction inlines it.
inline CStaticMesh::CStaticMesh() : mesh(0), lighting(1), fogging(1), animShapes(0), unknown10(-1), unknown14(-1) {}
#endif
