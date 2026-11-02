// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
extern "C" float sqrtf(float);
void CPlayerObject::Orthonormalize() { localToWorld.Orthonormalize(); }
void CPlayerObject::GetPosition(CVector3 &v) const { v = localToWorld.GetPos(); }
void CPlayerObject::GetCentroid(CVector3 &v) const {
    const Bounds *b = &bounds;
    CVector3 max(b->GetMax());
    const CVector3 &to = max;
    CVector3 from(b->GetMin());
    v.x = from.x + (to.x - from.x) * 0.5f;
    v.y = from.y + (to.y - from.y) * 0.5f;
    v.z = from.z + (to.z - from.z) * 0.5f;
}
void CPlayerObject::GetRightward(CVector3 &v) const { v = localToWorld.GetRight(); }
void CPlayerObject::GetForward(CVector3 &v) const { v = localToWorld.GetFront(); }
void CPlayerObject::GetUpward(CVector3 &v) const { v = localToWorld.GetUp(); }
void CPlayerObject::ApplyForceTo(const CVector3 &direction, float amount) {
    pathFlags |= 0x40000;
    float scale = amount * 0.001953125f;
    force = direction;
    float length = sqrtf(force.LengthSquared());
    if (length != 0.0f) force *= 1.0f / length;
    force *= scale;
}
