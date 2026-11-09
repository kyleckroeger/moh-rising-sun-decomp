// AI-assisted reconstruction from GR8E69. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

CAnimObject::LocomotionType CAnimObject::DetermineLocomotionType(float angle, LocomotionType current) {
    float degrees = angle * 57.2957763671875f;
    switch (current) {
    case 0:
        if (!(degrees <= -60.0f) && !(degrees >= 60.0f))
            return (LocomotionType)0;
        break;
    case 3:
        if (!(degrees <= -150.0f) && !(degrees >= -30.0f))
            return (LocomotionType)3;
        break;
    case 2:
        if (!(degrees <= 30.0f) && !(degrees >= 150.0f))
            return (LocomotionType)2;
        break;
    case 1:
        if (!(degrees <= 120.0f) && !(degrees >= -120.0f))
            return (LocomotionType)1;
        break;
    }
    if (!(degrees <= -45.0f) && degrees < 45.0f)
        return (LocomotionType)0;
    if (!(degrees <= -135.0f) && degrees < -45.0f)
        return (LocomotionType)3;
    if (!(degrees <= 45.0f) && degrees < 135.0f)
        return (LocomotionType)2;
    return (LocomotionType)1;
}


