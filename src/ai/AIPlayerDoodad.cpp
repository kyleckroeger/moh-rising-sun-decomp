// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. See include/game/AIDoodad.h; the file name is the original record.
#include "AIDoodad.h"
#include "SceneNode.h"

CAIPlayerDoodad::CAIPlayerDoodad() { filter = 0; }

CAIPlayerDoodad::~CAIPlayerDoodad() {
    if (filter)
        delete filter;
}

void CAIPlayerDoodad::Init(CPlayerObject *object) {
    if (!filter)
        filter = new ("source/ai_core/Engine_side/AIPlayerDoodad.cpp:69")
            CAIFilterPlayerObject((BSObject *)((ISceneNode *)object)->GetScriptObject());
    filter->LinkGameObject(this);
    player = object;
    CAIDoodad::Init();
    CAIFilterPlayerObject *playerFilter = (CAIFilterPlayerObject *)filter;
    playerFilter->SetOneTimeVars();
    playerFilter->UpdateManualUpdateVars();
    playerFilter->GetAIObject()->Init();
}

void CAIPlayerDoodad::CleanUp() {
    if (filter) {
        delete filter;
        filter = 0;
    }
}

CAIObject *CAIPlayerDoodad::GetAIObject() const { return filter->object; }
ISceneNode *CAIPlayerDoodad::GetSceneNode() const { return (ISceneNode *)player; }
