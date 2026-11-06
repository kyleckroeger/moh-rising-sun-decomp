// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "EAGLTransform.h"
#include <math.h>

void CAnimObject::Halt() {
    velocity.Zero();
    yawRate = 0.0f;
}

void CAnimObject::ApplyForceTo(const CVector3 &force, float strength) {
    if (attached || hurtlingBlocked)
        return;
    strength *= 0.001953125f;
    velocity = force;
    float length = sqrtf(velocity.LengthSquared());
    if (length != 0.0f)
        velocity *= 1.0f / length;
    velocity *= strength;
    BeginHurtling();
}

void CAnimObject::BeginHurtling() {
    if (!attached && !hurtlingBlocked) {
        hurtlingState = 1;
        hurtling = 0;
    }
}

void CAnimObject::GetTMLocalToWorld(EAGL::Transform &out) const { out = *(const EAGL::Transform *)&transform; }
void CAnimObject::GetTMLocalToWorld(CMatrix &out) const { out = transform; }
