// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
void CPlayerObject::Halt() {
    acceleration.Zero();
    velocity.Zero();
    field_28c = 0.0f;
}
void CPlayerObject::PreTransform(const CMatrix &m) {
    CMatrix::s_TempMat = localToWorld;
    localToWorld.Multiply(m, CMatrix::s_TempMat);
}
void CPlayerObject::Transform(const CMatrix &m) {
    CMatrix::s_TempMat = localToWorld;
    localToWorld.Multiply(CMatrix::s_TempMat, m);
}
