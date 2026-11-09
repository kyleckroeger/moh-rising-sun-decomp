// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "GameSettings.h"
#include "ScriptRuntime.h"
void CPlayerObject::SetHealthMax() {
    float *health = (float *)((BSObject *)((ISceneNode *)this)->GetScriptObject())->gameObject->GetScriptData();
    SMPPlayerSlot *slots = g_GameSettings.profile->players;
    *health = (float)slots[playerIndex].data.maxHealth;
}
