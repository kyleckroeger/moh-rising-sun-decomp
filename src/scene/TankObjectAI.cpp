// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Tank_object.cpp; member names are descriptive.
#include "AIFilterGlobal.h"
#include "SceneObjects.h"

void CTankObject::UpdateAI(float dt) {
    ((CAIObjectInterface *)aiObject)->Update(dt);
    if (pathNodeTimer <= 0) {
        g_aigAIFilterGlobalObject.EnableAllAStarPathNodes();
        g_aigAIFilterGlobalObject.DisableCollidingAStarPathNodes((const IVolume *)((ISceneNode *)this)->GetWorldBoundingVolume((ISceneNode::EVolumeType)0));
        pathNodeTimer = 60;
    } else {
        pathNodeTimer -= (int)dt;
    }
}
