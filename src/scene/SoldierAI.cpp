// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "AIDoodad.h"
#include "Weapon.h"

void CSoldierObject::InitAI() {
    ((CAISoldierDoodad *)aiDoodad)->Init(this);
    InitAI2();
}

void CSoldierObject::InitAI2() {
    CAISoldierDoodad *doodad = (CAISoldierDoodad *)aiDoodad;
    aiObject = (CAISoldierObject *)doodad->CAISoldierDoodad::GetAIObject();
    aiObject->SetSoldierObject(this);
    for (int i = 0; i < 4; i++) {
        if (weapons[i])
            weapons[i]->RegisterWithAI();
    }
}

CAIObject *CSoldierObject::GetAIObject() { return (CAIObject *)aiObject; }
