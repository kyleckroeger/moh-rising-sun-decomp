// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "profile.h"
#include <map>
#include <string.h>
#include <new>
#include "Weapon.h"
extern const int BUFSTART_PADSIZE;
extern const int BUFEND_PADSIZE;
void DebugMsg(const char *, ...);
// The factory header in front of each pooled object.
struct GameObjectEntry {
    int unknown_0;
    void *object; // +0x04; the object's game-object view.
    int id;       // +0x08; zero while the entry is free.
    bool IsUsed() const { return id != 0; }
};
class CGameObjectFactory {
  public:
    int maxObjects;  // +0x00
    int numObjects;  // +0x04
    int unknown_8;
    int objectSize;  // +0x0c
    char *objects;   // +0x10
    int nextId;      // +0x14
    unsigned char unknown_18[0x20 - 0x18];
    void CheckRecycleThreshold();
    template <class T> void *AllocateGameObject(T *&object);
};
// The game-object view of a weapon sits at +0x38 and starts with its factory entry.
static inline void *GameObjectView(CWeapon *weapon) {
    return weapon ? (char *)weapon + 0x38 : 0;
}
template <class T> inline void *CGameObjectFactory::AllocateGameObject(T *&object) {
    CheckRecycleThreshold();
    int stride = objectSize + BUFSTART_PADSIZE + BUFEND_PADSIZE;
    if (numObjects == maxObjects) {
        DebugMsg("Warning: CGameObjectFactory::AllocateGameObject: max number of objects (%d) are already allocated.\n", maxObjects);
    } else {
        for (int i = 0; i < maxObjects; i++) {
            GameObjectEntry *entry = (GameObjectEntry *)(objects + i * stride);
            if (!entry->IsUsed()) {
                numObjects++;
                nextId++;
                char *storage = (char *)entry + BUFSTART_PADSIZE;
                entry->id = nextId;
                entry->object = 0;
                memset(storage, 0, objectSize);
                object = new (storage) T;
                void *view = GameObjectView(object);
                entry->id = nextId;
                entry->object = view;
                *(GameObjectEntry **)((char *)object + 0x38) = entry;
                return GameObjectView(object);
            }
        }
    }
    return 0;
}
class CWeaponFactory {
  public:
    _STL::map<int, WeaponProperties_struct *> properties; // +0x00
    unsigned char unknown_10[0x20 - 0x10];
    CGameObjectFactory objects;                           // +0x20
    CWeapon *CreateWeapon(int, ISceneNode *, int);
    WeaponProperties_struct *GetWeaponPropertiesByCRC(int, int *);
    CWeapon *CreateWeaponByCRC(int, ISceneNode *, int);
};
CWeapon *CWeaponFactory::CreateWeapon(int type, ISceneNode *owner, int ammo) {
    CWeapon *weapon = 0;
    if (!objects.AllocateGameObject(weapon)) return 0;
    WeaponProperties_struct *props = properties.find(type)->second;
    weapon->Init(type, props, owner, ammo);
    return weapon;
}
WeaponProperties_struct *CWeaponFactory::GetWeaponPropertiesByCRC(int crc, int *index) {
    *index = -1;
    _STL::map<int, WeaponProperties_struct *>::iterator it = properties.find(crc);
    if (it != properties.end()) return it->second;
    return 0;
}
CWeapon *CWeaponFactory::CreateWeaponByCRC(int crc, ISceneNode *owner, int ammo) {
    int index = 0;
    WeaponProperties_struct *props = GetWeaponPropertiesByCRC(crc, &index);
    if (!props) {
        if (!crc) return 0;
        props = properties.begin()->second;
    }
    CWeapon *weapon = 0;
    if (!objects.AllocateGameObject(weapon)) return 0;
    weapon->Init(crc, props, owner, ammo);
    return weapon;
}
