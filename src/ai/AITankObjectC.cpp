// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AITankObject.cpp; status-variable meanings and member
// names are descriptive. Calls to CAIObject and its table convert the pointer unadjusted.
#include "AIBehavior.h"
#include "AIFilterGlobal.h"
#include "AIDoodad.h"

CAITankObject *CAITankObject::FindTank() {
    for (CAIObject *object = CAIObject::GetGlobalListStart(); object; object = object->nextGlobal) {
        if (((CAIObjectInterface *)object)->AsAITankObject())
            return ((CAIObjectInterface *)object)->AsAITankObject();
    }
    return 0;
}

CAITankObject *CAITankObject::AsAITankObject() { return this; }
