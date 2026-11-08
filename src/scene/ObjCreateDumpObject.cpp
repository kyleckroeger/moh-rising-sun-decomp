// AI-assisted reconstruction from GR8E69. Part of the original objcreate.cpp; local names are
// descriptive.
#include "SceneObjects.h"
#include "SceneNode.h"
#include "Sphere.h"
#include "MPWeaponSet.h"
#include "ObjCreate.h"

void DebugMsg(const char *, ...);
void ForceUpdateProximityTriggerStatus(BSGO_Basic *, bool);
void *AllocateSphereVolume();
class CStaticMesh {
  public:
    void GetCenter(CVector3 &) const;
};
class CStaticObjectFactory {
  public:
    static CStaticObject *AllocateStaticObject(int, int, PickupInfo *);
};
extern int g_bInMultiplayerMode;
extern float g_fDumpObjectCollisionRadius;

static inline void CopyMatrix(CMatrix &to, const CMatrix &from) { to = from; }

int CreateDumpObject(const CMatrix &matrix, int crc, bool keep, BSObject *script, int team, EPowerupType type) {
    PickupInfo pickup;
    CStaticObject *object = CStaticObjectFactory::AllocateStaticObject(crc, -1, &pickup);
    if (!object) {
        DebugMsg("\nERROR: Unable to create DumpObject type 0x%x.\n", crc);
        DebugMsg("The static object pool is all used up\n\n");
        return 0;
    }
    CStaticObjectInterface *self = (CStaticObjectInterface *)object;
    if (!g_bInMultiplayerMode) {
        self->SetTMLocalToWorld(matrix);
    } else {
        CMatrix raised;
        CopyMatrix(raised, matrix);
        raised.Translate(CVector3(0.0f, 0.0f, 0.2f));
        self->SetTMLocalToWorld(raised);
    }
    CStaticMesh *mesh = (CStaticMesh *)object->model;
    self->Init();
    CVolSphere *local = (CVolSphere *)AllocateSphereVolume();
    CVector3 center;
    mesh->GetCenter(center);
    if (!g_bInMultiplayerMode) {
        center.z += g_fDumpObjectCollisionRadius * 0.5f;
        local->center = center;
        local->SetRadius(g_fDumpObjectCollisionRadius);
    } else {
        local->center = center;
        local->SetRadius(2.0f * g_fDumpObjectCollisionRadius);
    }
    CVolSphere *world = (CVolSphere *)AllocateSphereVolume();
    world->radius = local->radius;
    world->center = local->center;
    object->SetLocalBoundingVolume((ISceneNode::EVolumeType)3, local);
    object->SetLocalBoundingVolume((ISceneNode::EVolumeType)0, local);
    object->SetWorldBoundingVolume((ISceneNode::EVolumeType)3, world);
    object->SetWorldBoundingVolume((ISceneNode::EVolumeType)0, world);
    g_scene.Add(*(ISceneNode *)object, 0);
    self->SetCollisionId((EClsnId)14);
    if (team > 0)
        pickup.value00 = team;
    pickup.script = script;
    if (type)
        pickup.value20 = type;
    g_DumpObjectContainer.AddDumpObject(object, pickup, keep);
    object->flags.dumpObject = 1;
    BSGO_Dummy dummy;
    dummy.proximityData[1] = 0;
    dummy.proximityData[0] = 0;
    dummy.proximityData[3] = 0;
    dummy.proximityData[2] = 0;
    dummy.script = 0;
    dummy.sceneNode.Assign((ISceneNode *)object);
    ForceUpdateProximityTriggerStatus(&dummy, true);
    return (int)object;
}
