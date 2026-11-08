// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"

class CStaticObjectFactory {
  public:
    static void DeallocateStaticObject(CStaticObject *);
};

CStaticObject *CSoldierObject::DetachObject(char *name) {
    int slot = GetAttachPoint(name);
    if (slot < 0)
        return 0;
    CStaticObject *object = attachSlots[slot].object;
    if (object) {
        if (attachSlots[slot].addToScene == 1)
            g_scene.Add(*(ISceneNode *)object, 0);
        if (attachSlots[slot].owned) {
            CStaticObjectFactory::DeallocateStaticObject(attachSlots[slot].object);
            object = 0;
        }
    }
    attachSlots[slot].object = 0;
    attachSlots[slot].active = 0;
    attachSlots[slot].addToScene = 0;
    attachSlots[slot].owned = 0;
    if (emitterSlot == slot)
        emitterActive = 0;
    return object;
}
