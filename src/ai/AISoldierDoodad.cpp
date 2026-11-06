// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. See include/game/AIDoodad.h; the file name is the original record.
#include "AIDoodad.h"
#include "SceneNode.h"
#include "SceneObjects.h"

CAISoldierDoodad::CAISoldierDoodad() {}

CAISoldierDoodad::~CAISoldierDoodad() {
    if (filter)
        delete filter;
}

void CAISoldierDoodad::Init(CBotObject *object) {
    filter = new ((char *)object + 8000)
        CAIFilterSoldierObject((BSObject *)((ISceneNode *)object)->GetScriptObject());
    filter->LinkGameObject(this);
    soldier = (CSoldierObject *)object;
    Init();
}

void CAISoldierDoodad::Init(CSoldierObject *object) {
    filter = new ((char *)object + 0x1610)
        CAIFilterSoldierObject((BSObject *)((ISceneNode *)object)->GetScriptObject());
    filter->LinkGameObject(this);
    soldier = object;
    Init();
}

void CAISoldierDoodad::Init() {
    CAIDoodad::Init();
    CAIFilterSoldierObject *soldierFilter = (CAIFilterSoldierObject *)filter;
    soldierFilter->SetOneTimeVars();
    soldierFilter->UpdateManualUpdateVars();
    soldierFilter->GetAIObject()->Init();
}

void CAISoldierDoodad::CleanUp() {
    if (filter) {
        delete filter;
        filter = 0;
    }
}

CAIObject *CAISoldierDoodad::GetAIObject() const { return filter->object; }
ISceneNode *CAISoldierDoodad::GetSceneNode() const { return (ISceneNode *)soldier; }
void CAISoldierDoodad::SetUpperTranslation(const CVector3 &v) { soldier->SetUpperTranslation(v); }
void CAISoldierDoodad::SetUpperXAngleDifference(float a, float b) { soldier->SetUpperXAngleDifference(a, b); }
void CAISoldierDoodad::SetUpperZAngleDifference(float a, float b) { soldier->SetUpperZAngleDifference(a, b); }
