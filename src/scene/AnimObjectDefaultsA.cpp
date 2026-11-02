// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

void CAnimObject::LineAlmostCollided(CBullet *) const {}
void CAnimObject::Shutdown() {}
void CAnimObject::SetScript(BSObject *s) { script = s; }
void CAnimObject::UpdateLocalBoundingVolume(ISceneNode::EVolumeType, const CVector3 &, const CVector3 &) {}
void CAnimObject::SetCollisionId(EClsnId id) { collisionId = id; }
CAnimObject *CAnimObject::AsAnimObject() { return this; }
const CAnimObject *CAnimObject::AsAnimObject() const { return this; }
BSObject *CAnimObject::GetScriptObject() const { return script; }
CVector3 CAnimObject::GetWorldLinearVelocity() const { return velocity; }
