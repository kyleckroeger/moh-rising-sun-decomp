// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original AITankObject.cpp; member names are descriptive.
#include "AIBehavior.h"
#include "AIFilterGlobal.h"
#include "AIDoodad.h"

int CAITankObject::ChooseTarget(float) {
    CAIObject *self = (CAIObject *)this;
    for (CAIObject *other = CAIObject::GetGlobalListStart(); other; other = other->nextGlobal) {
        if (other != self && other->team != self->team) {
            self->targeting.SetTarget(other);
            return ((CAIObjectInterface *)other)->GetSceneNode()->GetScriptObject();
        }
    }
    self->targeting.SetTarget(0);
    return 0;
}
