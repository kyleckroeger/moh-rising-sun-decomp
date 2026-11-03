// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"

int CAIObject::IsObjectInFieldOfView(CAIObject &object, float range, float angle) {
    if (object.inShadow)
        range *= object.shadowAmount;
    if (((CAIObjectInterface *)&object)->IsCrouching())
        range *= 0.75f;
    return IsObjectInFieldOfView(object.position, range, angle, ((CAIObjectInterface *)&object)->IsCrouching(), 0);
}

// The same body is inlined into its callers in AIObjectLineOfSight.cpp; this is the out-of-line copy.
int CAIObject::IsObjectInLineOfSight(const CAITarget &target, float range) {
    bool crouching = false;
    CAIObject *object = target.aiObject;
    if (object) {
        if (object->inShadow)
            range *= object->shadowAmount;
        crouching = ((CAIObjectInterface *)object)->IsCrouching();
        if (crouching)
            range *= 0.75f;
    }
    return ((CAIObjectInterface *)this)->IsObjectInLineOfSight(target.GetPosition(), range, crouching, &target);
}
