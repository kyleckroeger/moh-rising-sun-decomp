// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "CameraShake.h"
#include "SceneObjects.h"
class CWeaponFactory {
  public:
    CWeapon *CreateWeapon(int, ISceneNode *, int);
};
extern CWeaponFactory g_WeaponFactory;
void CPlayerWeaponObject::Init(int type, int index, CPlayerObject *owner) {
    player = owner;
    transform.GetSlot(0);
    flag31 = 0;
    visible = 0;
    enabled = 0;
    flag28 = 0;
    field_3f4 = -1;
    weapon = g_WeaponFactory.CreateWeapon(type, (ISceneNode *)owner, index);
    g_scene.Add(*(ISceneNode *)this, 0);
    muzzleFlash = 0;
    field_370 = CVector3(0.0f, 0.0f, 0.0f);
    state = 0;
}
