// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "AnimatedVolume.h"
#include "EAGLTransform.h"

// Method-only views; the symbols establish the nesting.
namespace EALA {
namespace Character {
class Choreographer {
  public:
    void EvalPose(float);
};
class Skeleton {
  public:
    void GetBoundingVolume(COORD3 &, COORD3 &) const;
};
}
}

void CAnimObject::UpdateAnimation(float dt) {
    animTurned = 0;
    yawRate = 0.0f;
    animMove.Zero();
    int done = 1;
    animMoveScale = 1.0f;
    ((EALA::Character::Choreographer *)choreographer)->EvalPose(dt * 0.0166666675f);
    float inverse = 1.0f / dt;
    animMoved = 0;
    poseEvaluated = done;
    if (!hurtlingState && !hurtlingBlocked) {
        float s = scale;
        velocity.x = (animMoveScale * velocity.x + animMove.x) * s * inverse;
        velocity.y = (animMoveScale * velocity.y + animMove.y) * s * inverse;
        if (!gravity)
            velocity.z = velocity.z * s * inverse;
        animMoved = done;
    } else if (hurtlingBlocked) {
        velocity.Zero();
    }
    yawRate *= inverse;
    if (animatedVolume)
        animatedVolume->collisionFlag = 0;
    CVector3 min, max;
    ((const EALA::Character::Skeleton *)skeleton)->GetBoundingVolume(*(COORD3 *)&min, *(COORD3 *)&max);
    float s = scale;
    min *= s;
    max *= s;
    ((CAnimObjectInterface *)this)->UpdateLocalBoundingVolume((ISceneNode::EVolumeType)3, min, max);
}
