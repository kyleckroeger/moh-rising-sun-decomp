// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Tank_object.cpp; member names are descriptive.
#include "AIFilterGlobal.h"
#include "SceneObjects.h"
#include "AIBehavior.h"
#include "AIDoodad.h"
#include "Weapon.h"

class CWeaponFactory {
  public:
    void DestroyWeapon(CWeapon *);
};
extern CWeaponFactory g_WeaponFactory;

void CTankObject::InitAI() {
    CAITankDoodad *doodad = (CAITankDoodad *)aiDoodad;
    doodad->Init(this);
    CAITankObject *ai = (CAITankObject *)doodad->CAITankDoodad::GetAIObject();
    aiObject = ai;
    ai->sceneNode = (ISceneNode *)this;
}

CAIObject *CTankObject::GetAIObject() { return (CAIObject *)aiObject; }

void CTankObject::CleanUp() {
    g_aigAIFilterGlobalObject.EnableAllAStarPathNodes();
    ((CAITankDoodad *)aiDoodad)->CleanUp();
    for (int i = 0; i < 3; i++) {
        if (weapons[i])
            g_WeaponFactory.DestroyWeapon(weapons[i]);
    }
    CHierObject::CleanUp();
}

void CTankObject::BeginUpdate(float dt) {
    CHierObject::BeginUpdate(dt);
    for (int i = 0; i < 3; i++) {
        if (weapons[i])
            weapons[i]->Update(dt, 0);
    }
    ((CAIObjectInterface *)aiObject)->PreUpdate();
}
