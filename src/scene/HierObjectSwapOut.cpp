// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "StaticMesh.h"
#include "Box.h"
#include "BoxSetters.h"

extern "C" float sqrtf(float);

struct MotionData {
    int unknown_0;
    int rate;
    int loop;
    int hasMotion;
    MotionFrame frames[1];
};

static inline float Length(const CVector3 &v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

static inline void Normalize(CVector3 &v) {
    float length = Length(v);
    if (length != 0.0f)
        v *= 1.0f / length;
}

static inline void SetHalfHeight(CVolBox *box, float h) { box->halfSize[2] = h * 0.5f; }

void CHierObject::SwapOut(CStaticMesh *mesh, MotionData *motion) {
    model = mesh;
    if (motion) {
        CStaticAnimMesh *anim = (CStaticAnimMesh *)mesh;
        motionFrameCount = anim->GetNumFrames();
        motionSpeed = anim->GetTicksPerFrame();
        motionLoop = motion->loop;
        motionRate = motion->rate;
        flags.hasMotion = motion->hasMotion != 0;
        motionFrames = motion->frames;
    } else {
        motionFrameCount = 0;
        motionSpeed = 0;
        motionLoop = 0;
        motionRate = 0.0f;
        flags.hasMotion = 0;
        motionFrames = 0;
    }
    CVector3 x, y, z, center;
    ((CStaticMesh *)model)->GetBasis(x, y, z);
    ((CStaticMesh *)model)->GetCenter(center);
    CVolBox *box = (CVolBox *)worldVolume;
    if (((CStaticObjectInterface *)root)->AsHierTankObject()) {
        // The compiler keeps the call; -ffast-math folds the zero product away.
        center.x += 0.0f * Length(x);
        center.y += 0.1f * Length(z);
        center.z += -0.1f * Length(y);
        box->SetWidth(1.6f * Length(x));
        SetHalfHeight(box, 1.7f * Length(z));
        box->SetDepth(1.4f * Length(y));
    } else {
        box->SetWidth(2.0f * Length(x));
        SetHalfHeight(box, 2.0f * Length(z));
        box->SetDepth(2.0f * Length(y));
    }
    Normalize(x);
    Normalize(y);
    Normalize(z);
    box->SetBasis(x, y, z);
    box->SetCenter(center);
}
