// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "profile.h"
#include <map>
#include "CameraShake.h"
#include "SceneObjects.h"
typedef _STL::map<int, CPlayerWeaponObject *> PlayerWeaponMap;
// CPlayerObject's view keeps the map as raw storage so that other users need no STL headers.
#define WEAPONS ((PlayerWeaponMap *)weaponMap)
enum EObjectClass { ObjectClassUnknown = 0 };
class CAnimObjectFactory {
  public:
    CAnimObject *AllocateObject(unsigned int, EObjectClass);
};
extern CAnimObjectFactory g_AnimObjFactory;
CPlayerWeaponObject *CPlayerObject::AddPlayerWeapon(int crc) {
    CPlayerWeaponObject *object = 0;
    if (WEAPONS->find(crc) == WEAPONS->end()) {
        object = (CPlayerWeaponObject *)g_AnimObjFactory.AllocateObject(crc, (EObjectClass)2);
        if (object) {
            object->Init(crc, 0, this);
            object->enabled = 0;
            ((CAnimObject *)object)->SetLocalBoundingVolume((ISceneNode::EVolumeType)3, (IVolume *)boxStorage);
            ((CAnimObject *)object)->SetWorldBoundingVolume((ISceneNode::EVolumeType)3, (IVolume *)(boxStorage + 0x50));
            (*WEAPONS)[crc] = object;
            if (!weaponList) {
                weaponList = object;
                object->previous = object;
                object->next = object;
            } else {
                CPlayerWeaponObject *entry = weaponList;
                int order = object->weapon->properties->weaponType;
                int first = entry->weapon->properties->weaponType;
                if (order < first) {
                    object->previous = entry->previous;
                    object->next = entry;
                    entry->previous->next = object;
                    entry->previous = object;
                    weaponList = object;
                } else if (order > entry->previous->weapon->properties->weaponType) {
                    object->next = entry;
                    object->previous = entry->previous;
                    entry->previous = object;
                    object->previous->next = object;
                } else {
                    if (order > first) {
                        do entry = entry->next;
                        while (order > entry->weapon->properties->weaponType);
                    }
                    CPlayerWeaponObject *before = entry->previous;
                    entry->previous = object;
                    before->next = object;
                    object->previous = before;
                    object->next = entry;
                }
            }
        }
    }
    return object;
}
