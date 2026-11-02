// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. A fragment of the original source/ai_core/Filter/AIFilter.cpp; member names
// and the trigger-field meanings are descriptive.
#include "AIDoodad.h"
#include "SceneObjects.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "Weapon.h"

CAIFilterObject::CAIFilterObject(BSObject *script) { scriptObject = script; }

CAIFilterObject::~CAIFilterObject() {}

void CAIFilterObject::LinkGameObject(void *game) { gameObject = game; }

CAIFilterSoldierObject::CAIFilterSoldierObject(BSObject *script) : CAIFilterObject(script) {
    char *place = (char *)this + 20;
    if ((unsigned int)place & 15)
        place = (char *)((unsigned int)place & ~15) + 16;
    object = (CAIObject *)new (place) CAISoldierObject(this);
}

CAIFilterSoldierObject::~CAIFilterSoldierObject() {
    if (object)
        delete (CAIObjectInterface *)object;
}

void *CAIFilterSoldierObject::operator new(unsigned int, void *place) { return place; }

void CAIFilterSoldierObject::operator delete(void *) {}
