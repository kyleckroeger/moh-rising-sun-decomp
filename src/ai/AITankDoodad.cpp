// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. See include/game/AIDoodad.h; the file name is the original record.
#include "AIDoodad.h"
#include "SceneNode.h"

CAITankDoodad::CAITankDoodad() {}

CAITankDoodad::~CAITankDoodad() {
    if (filter)
        delete filter;
}

void CAITankDoodad::Init(CTankObject *object) {
    filter = new ((char *)object + 0xbf0)
        CAIFilterTankObject((BSObject *)((ISceneNode *)object)->GetScriptObject());
    filter->LinkGameObject(this);
    tank = object;
    CAIDoodad::Init();
    CAIFilterTankObject *tankFilter = (CAIFilterTankObject *)filter;
    tankFilter->SetOneTimeVars();
    tankFilter->UpdateManualUpdateVars();
    tankFilter->GetAIObject()->Init();
}

void CAITankDoodad::CleanUp() {
    if (filter) {
        delete filter;
        filter = 0;
    }
}

CAIObject *CAITankDoodad::GetAIObject() const { return filter->object; }
ISceneNode *CAITankDoodad::GetSceneNode() const { return (ISceneNode *)tank; }
