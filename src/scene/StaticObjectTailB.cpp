// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

void CStaticObject::SetScript(BSObject *s) { script = s; }
void CStaticObject::SetCollisionId(EClsnId id) { field_20c = id; }
void CStaticObject::SetDrawEnabled(bool enabled) { flags.drawEnabled = enabled; }
int CStaticObject::GetObjectType() const { return 0; }
CVector3 CStaticObject::GetWorldLinearVelocity() const { return velocity; }
CStaticObject *CStaticObject::AsStaticObject() { return this; }
const CStaticObject *CStaticObject::AsStaticObject() const { return this; }
BSObject *CStaticObject::GetScriptObject() const { return script; }
unsigned int CStaticObject::IsDrawEnabled() const { return flags.drawEnabled; }
void CStaticObject::SetLightVolumeTransitionDuration(float duration) { lightVolumes.transitionDuration = duration; }
void *CStaticObject::AsThrownObject() { return 0; }
void *CStaticObject::AsWeaponObject() { return 0; }
CLight *CStaticObject::GetAttachedLight() const { return attachedLight; }
CTankObject *CStaticObject::AsHierTankObject() { return 0; }

