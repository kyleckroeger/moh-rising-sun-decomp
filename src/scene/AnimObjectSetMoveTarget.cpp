// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
extern "C" float atan2f(float, float);

void CAnimObject::SetMoveTarget(const CVector3 &target) {
    moveTarget = target;
    moveTargetPending = 1;
    if (animCallback == 1) {
        CVector3 direction(moveTarget);
        direction -= transform.GetPos();
        float heading = atan2f(-direction.x, direction.y);
        CAIObject *ai = ((CAnimObjectInterface *)this)->GetAIObject();
        float turn;
        if (ai && ai->targeting.HasValidTarget()) {
            direction = ai->targeting.GetTarget().GetPosition();
            direction -= transform.GetPos();
            turn = heading - atan2f(-direction.x, direction.y);
            float low = -3.1415927410125732f, high = 3.1415927410125732f;
            if (!(turn <= high)) {
                do {
                    turn -= 6.2831854820251465f;
                } while (turn > high);
            }
            if (!(turn >= low)) {
                do {
                    turn += 6.2831854820251465f;
                } while (turn < low);
            }
        } else {
            turn = heading - yaw;
            float low = -3.1415927410125732f, high = 3.1415927410125732f;
            if (!(turn <= high)) {
                do {
                    turn -= 6.2831854820251465f;
                } while (turn > high);
            }
            if (!(turn >= low)) {
                do {
                    turn += 6.2831854820251465f;
                } while (turn < low);
            }
        }
        LocomotionType type = DetermineLocomotionType(turn, (LocomotionType)locomotionType);
        if (type != locomotionType) {
            locomotionType = type;
            ((CAnimObjectInterface *)this)->SetAnimationState(stateName, stateValue);
        }
    } else {
        locomotionType = 0;
    }
}
