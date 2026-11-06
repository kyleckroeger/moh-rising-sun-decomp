// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ScriptRuntime.h"
#include "BSGameObject.h"

void CSoldierObject::Destroy() {
    if (scriptObject && scriptObject->gameObject)
        scriptObject->gameObject->Destroy();
}
