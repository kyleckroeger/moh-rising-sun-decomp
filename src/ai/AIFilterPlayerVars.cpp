// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. A fragment of the original source/ai_core/Filter/AIFilter.cpp; member names
// and the trigger-field meanings are descriptive.
#include "AIDoodad.h"
#include "SceneObjects.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "Weapon.h"

// The trigger-field names are shared literals in the original file's pool, owned with the
// soldier function; each is mapped back to its original address in the manifest.
static const char fieldTeam[] __attribute__((section(".rodata.field_team"), aligned(4))) = { 'T', 'e', 'a', 'm', 0 };
static const char fieldID[] __attribute__((section(".rodata.field_id"), aligned(4))) = { 'I', 'D', 0 };
static const char fieldHitPoints[] __attribute__((section(".rodata.field_hitpoints"), aligned(4))) = { 'H', 'i', 't', 'P', 'o', 'i', 'n', 't', 's', 0 };

void CAIFilterPlayerObject::SetOneTimeVars() {
    CAIPlayerDoodad *doodad = (CAIPlayerDoodad *)gameObject;
    CAIObject *ai = object;
    scriptObject = (BSObject *)doodad->GetSceneNode()->GetScriptObject();
    bsgo = *(void **)((char *)scriptObject + 0x20);
    BSObject *script = (BSObject *)((ISceneNode *)doodad->player)->GetScriptObject();
    ai->team = script->nativeObject->GetInt(fieldTeam);
    ai->unknown599 = 5;
    ai->id = script->nativeObject->GetInt(fieldID);
    ai->hitPoints = ai->maxHitPoints = script->nativeObject->GetInt(fieldHitPoints);
    ((CAIObjectInterface *)ai)->UpdateFromGameObject((IMovingSceneNode *)doodad->GetSceneNode());
}

void CAIFilterPlayerObject::UpdateManualUpdateVars() {}
