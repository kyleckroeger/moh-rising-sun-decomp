// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
int CPlayerObject::GetScriptObject() const { return (int)script; }
CAIDoodad *CPlayerObject::GetAIDoodad() { return (CAIDoodad *)aiDoodad; }
const CAIDoodad *CPlayerObject::GetAIDoodad() const { return (const CAIDoodad *)aiDoodad; }
CPlayerObject *CPlayerObject::AsPlayerObject() { return this; }
const CPlayerObject *CPlayerObject::AsPlayerObject() const { return this; }
void CPlayerObject::SetCollisionId(EClsnId id) { collisionId = id; }
CVector3 CPlayerObject::GetWorldLinearVelocity() const { return velocity; }
const char *CPlayerObject::GetName() { return name; }
unsigned char CPlayerObject::GetPlayerIndex() { return playerIndex; }
int CPlayerObject::GetTeam() { return team; }
void CPlayerObject::Destroy() { delete (ISceneNode *)this; }
