// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "IDestructible.h"
#include "ObserverTypes.h"
#include "ObjCreate.h"

void FreeSphereVolume(void *);
class TriggerObject;
void *CreateCollisionVolume(TriggerObject *, int);
void *CreatePlayer(TriggerObject *, int);
void *CreateDestructable(TriggerObject *, int, int);
class CStaticObjectFactory {
  public:
    static void DeallocateStaticObject(CStaticObject *);
};

DumpObject *HitDumpObject(CStaticObject *object, PickupInfo *pickup) {
    DumpObject *dump = g_DumpObjectContainer.GetDumpObject(object);
    if (!dump) {
        pickup->value00 = 0;
        pickup->value04 = 0;
        pickup->value14 = 0;
        pickup->value20 = 0;
        pickup->value08 = 0;
        pickup->value0c = 0;
        pickup->script = 0;
        return 0;
    }
    *pickup = dump->pickup;
    return dump;
}

void MarkDumpObjectForDestruction(DumpObject *dump) {
    ((ISubject *)dump->object)->MarkForDestruction(0);
    g_DumpObjectContainer.DeleteDumpObject(dump);
}

void MarkDumpObjectForDestruction(CStaticObject *object) {
    DumpObject *dump = g_DumpObjectContainer.GetDumpObject(object);
    if (dump)
        MarkDumpObjectForDestruction(dump);
}

void DestroyDumpObject(CStaticObject *object) {
    ISceneNode *node = (ISceneNode *)object;
    if (node->GetLocalBoundingVolume((ISceneNode::EVolumeType)3))
        FreeSphereVolume((void *)node->GetLocalBoundingVolume((ISceneNode::EVolumeType)3));
    if (node->GetWorldBoundingVolume((ISceneNode::EVolumeType)3))
        FreeSphereVolume((void *)node->GetWorldBoundingVolume((ISceneNode::EVolumeType)3));
    CStaticObjectFactory::DeallocateStaticObject(object);
}

void DestroyOldestDumpObject() {
    if (g_DumpObjectContainer.count > 0)
        g_DumpObjectContainer.DeleteOldest(false);
}

void *CreateEnvModMechanic(TriggerObject *trigger, int index) { return CreateCollisionVolume(trigger, index); }

void *CreateMechanic(TriggerObject *trigger, int index, int type, void *data) {
    void *object;
    if (type == (int)0xfd092133 || type == 0x271bd574)
        object = CreatePlayer(trigger, (int)data);
    else
        object = CreateDestructable(trigger, index, type);
    return object;
}
