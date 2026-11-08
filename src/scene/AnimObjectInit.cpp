// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "../game-containers/TargetTypes.h"

namespace EALA {
namespace Character {
class Choreographer {
  public:
    void SetOpcodeHandler(Opcode::Handler *);
};
}
}

void CAnimObject::Init() {
    AO_OpcodeHandler *handler = &opcodeHandler;
    handler->owner = this;
    field_44 = 0;
    field_48 = 0;
    animatedVolume = 0;
    transform.GetSlot(0);
    velocity.Zero();
    previousPosition.Zero();
    animMove.Zero();
    yaw = 0.0f;
    attachBlend = -1.0f;
    localVolumeType3 = 0;
    localVolumeType0 = 0;
    worldVolumeType3 = 0;
    worldVolumeType0 = 0;
    collisionId = -1;
    script = 0;
    attached = 0;
    attachedToLocation = 0;
    allowAnimationTranslation = 1;
    ((WeakPtr<ISceneNode, 8> *)weakNode)->Assign(0);
    moveTargetPending = 0;
    gravity = 1;
    groundCheck = 0;
    fallStarted = 0;
    hurtlingState = 0;
    hurtling = 1;
    field_2c0 = 40;
    ((EALA::Character::Choreographer *)choreographer)->SetOpcodeHandler(handler);
    animCallback = -1;
    pendingCallback = -1;
    poseEvaluated = 0;
    stateName = 0;
    locomotionType = 0;
    stateDone = 1;
}
