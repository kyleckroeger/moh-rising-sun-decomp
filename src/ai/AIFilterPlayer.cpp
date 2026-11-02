// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. A fragment of the original source/ai_core/Filter/AIFilter.cpp; member names
// and the trigger-field meanings are descriptive.
#include "AIDoodad.h"
#include "SceneObjects.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "Weapon.h"

CAIFilterPlayerObject::~CAIFilterPlayerObject() {
    if (object)
        delete (CAIObjectInterface *)object;
}
