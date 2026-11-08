// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "PlacementNew.h"
#include "ScriptPool.h"
#include "BoxSetters.h"
#include "ObjCreate.h"

class CStaticMesh {
  public:
    void GetBasis(CVector3 &, CVector3 &, CVector3 &) const;
    void GetCenter(CVector3 &) const;
};
class CStaticObjectFactory {
  public:
    static CStaticObject *AllocateStaticObject(int, int, PickupInfo *);
};
void *AllocateBoxVolume();

static inline float Length(const CVector3 &v) {
    float sum = v.x * v.x + v.y * v.y;
    sum += v.z * v.z;
    return sqrtf(sum);
}
static inline void Normalize(CVector3 &v) {
    float length = Length(v);
    if (length != 0.0f) {
        float scale = 1.0f / length;
        v.x *= scale;
        v.y *= scale;
        v.z *= scale;
    }
}

void CreateSubHierObject(CHierObject *parent, int sub, int index, unsigned int attachment, int, int type) {
    BSObject *script = (BSObject *)((ISceneNode *)parent)->GetScriptObject();
    CHierObject *object = ((ISceneNode *)CStaticObjectFactory::AllocateStaticObject(type, sub, 0))->AsHierObject();
    void *tank = ((CStaticObjectInterface *)parent)->AsHierTankObject();
    CVolBox *box = (CVolBox *)AllocateBoxVolume();
    CStaticMesh *mesh = (CStaticMesh *)((CStaticObject *)object)->model;
    CVector3 a, b, c, center;
    mesh->GetBasis(a, b, c);
    mesh->GetCenter(center);
    if (tank) {
        Length(a); // The result is unused in the original too.
        center.y += 0.1f * Length(c);
        center.z += -0.1f * Length(b);
        box->SetWidth(1.8f * Length(a));
        // Halved here, unlike the player box height through SetHeight.
        box->halfSize[2] = 1.9f * Length(c) * 0.5f;
        box->SetDepth(1.6f * Length(b));
    } else {
        box->SetWidth(2.0f * Length(a));
        box->halfSize[2] = 2.0f * Length(c) * 0.5f;
        box->SetDepth(2.0f * Length(b));
    }
    Normalize(a);
    Normalize(b);
    Normalize(c);
    box->SetBasis(a, b, c);
    box->SetCenter(center);
    ((CStaticObject *)object)->SetLocalBoundingVolume((ISceneNode::EVolumeType)3, box);
    ((CStaticObject *)object)->SetLocalBoundingVolume((ISceneNode::EVolumeType)0, box);
    CVolBox *world = (CVolBox *)AllocateBoxVolume();
    *world = *box;
    ((CStaticObject *)object)->SetWorldBoundingVolume((ISceneNode::EVolumeType)3, world);
    ((CStaticObject *)object)->SetWorldBoundingVolume((ISceneNode::EVolumeType)0, world);
    ((CStaticObjectInterface *)object)->SetScript(script);
    CVector3 offset(0.0f, 0.0f, 0.0f);
    object->Init(parent, sub, index, attachment, offset);
}
