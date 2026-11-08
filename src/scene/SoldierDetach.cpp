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
    // The original forms this + slot * 112 first; the source form of the address is inferred.
    CSoldierObject *base = (CSoldierObject *)((char *)this + slot * 112);
    CStaticObject *object = base->attachSlots[0].object;
    if (object) {
        if (base->attachSlots[0].addToScene == 1)
            g_scene.Add(*(ISceneNode *)object, 0);
        if (base->attachSlots[0].owned) {
            CStaticObjectFactory::DeallocateStaticObject(base->attachSlots[0].object);
            object = 0;
        }
    }
    CSoldierObject *cleared = (CSoldierObject *)((char *)this + slot * 112);
    cleared->attachSlots[0].object = 0;
    cleared->attachSlots[0].active = 0;
    cleared->attachSlots[0].addToScene = 0;
    cleared->attachSlots[0].owned = 0;
    if (emitterSlot == slot)
        emitterActive = 0;
    return object;
}
