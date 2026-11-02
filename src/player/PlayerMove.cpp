// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
void CPlayerObject::SetPosition(const CVector3 &v) { localToWorld.SetPos(v); }
void CPlayerObject::SetBasis(const CVector3 &right, const CVector3 &front, const CVector3 &up) {
    localToWorld.SetRight(right);
    localToWorld.SetFront(front);
    localToWorld.SetUp(up);
}
void CPlayerObject::Move(const CVector3 &v) { localToWorld.Translate(v); }
void CPlayerObject::Rotate(const CVector3 &axis, float angle) { localToWorld.Rotate(axis, angle); }
void CPlayerObject::Pitch(float angle) { pitch += angle; }
void CPlayerObject::Roll(float) {}
