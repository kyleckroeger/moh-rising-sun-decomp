// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AITankObject.cpp; status-variable meanings and member
// names are descriptive. Calls to CAIObject and its table convert the pointer unadjusted.
#include "AIBehavior.h"
#include "AIFilterGlobal.h"
#include "AIDoodad.h"

void CAITankObject::SetupTargetMatchList(aistatus_match *list, int count) {
    aistatus_match *out = targetMatchList;
    for (; count > 0; count--)
        *out++ = *list++;
}
