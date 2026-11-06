// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "ScriptRuntime.h"
float CPlayerObject::GetHitpoints() const {
    BSObject *object = (BSObject *)((const ISceneNode *)this)->GetScriptObject();
    return *(float *)object->gameObject->GetScriptData();
}
