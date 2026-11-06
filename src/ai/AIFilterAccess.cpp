// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. A fragment of the original source/ai_core/Filter/AIFilter.cpp; member names
// and the trigger-field meanings are descriptive.
#include "AIDoodad.h"
#include "SceneObjects.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "Weapon.h"

BSObject *CAIFilterObject::GetScriptObject() const { return scriptObject; }
void *CAIFilterObject::GetBSGO() const { return bsgo; }
void CAIFilterObject::SetMoveTarget(const CVector3 &) {}

void CAIFilterSoldierObject::SetMoveTarget(const CVector3 &target) {
    ((CAnimObjectInterface *)GetAIDoodad()->soldier)->SetMoveTarget(target);
}

CAISoldierDoodad *CAIFilterSoldierObject::GetAIDoodad() const { return (CAISoldierDoodad *)gameObject; }
CAISoldierObject *CAIFilterSoldierObject::GetAIObject() const { return (CAISoldierObject *)object; }
CAITankObject *CAIFilterTankObject::GetAIObject() const { return (CAITankObject *)object; }
CAIPlayerObject *CAIFilterPlayerObject::GetAIObject() const { return (CAIPlayerObject *)object; }
