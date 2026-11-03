// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"
#include "AIMovement.h"
#include "BSGameObject.h"
#include "ScriptRuntime.h"
#include <stdio.h>
#include <string.h>

extern int g_iMsgVerbosityLevel;

float CAIObject::GetDistanceSquaredXYReal(CAITarget target) {
    CVector3 location = target.GetPosition();
    return position.DistanceSquaredXY(location);
}

int CAIObject::ComputeStatusDistance(CAITarget *target) {
    CVector3 location = target->GetPosition();
    float distance = position.DistanceXY(location) * 0.1f;
    return !(distance >= 15.0f) ? (int)distance : 15;
}

int CAIObject::ComputeStatusExponentialDistance(CAITarget *target) {
    CVector3 location = target->GetPosition();
    float distance = position.Distance(location);
    if (!(distance >= 64.0f)) {
        if (!(distance >= 16.0f)) {
            if (!(distance >= 4.0f))
                return distance >= 1.0f;
            return !(distance < 9.0f) ? 3 : 2;
        }
        if (!(distance >= 36.0f))
            return !(distance < 25.0f) ? 5 : 4;
        return !(distance < 49.0f) ? 7 : 6;
    }
    if (!(distance >= 144.0f)) {
        if (!(distance >= 100.0f))
            return !(distance < 81.0f) ? 9 : 8;
        return !(distance < 121.0f) ? 11 : 10;
    }
    if (distance < 196.0f) {
        if (distance < 169.0f)
            return 12;
        return 13;
    }
    if (distance < 225.0f)
        return 14;
    return 15;
}

