// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

EClsnId CStaticObject::GetCollisionId() const { return (EClsnId)field_20c; }

void CStaticObject::GetTMLocalToWorld(CMatrix &out) const { out = transform; }

void CStaticObject::GetPosition(CVector3 &out) const { out = transform.GetPos(); }

void CStaticObject::GetRightward(CVector3 &out) const { out = transform.GetRight(); }

void CStaticObject::GetForward(CVector3 &out) const { out = transform.GetFront(); }

void CStaticObject::GetUpward(CVector3 &out) const { out = transform.GetUp(); }

void CStaticObject::SetTMLocalToWorld(const CMatrix &matrix) {
    transform = matrix;
    orientation.SetFromMatrix(matrix);
}

void CStaticObject::SetPosition(const CVector3 &position) { transform.SetPos(position); }

void CStaticObject::SetBasis(const CVector3 &right, const CVector3 &front, const CVector3 &up) {
    CMatrix *matrix = &transform;
    matrix->SetRight(right);
    matrix->SetFront(front);
    matrix->SetUp(up);
    orientation.SetFromMatrix(*matrix);
}

void CStaticObject::EnterLightVolume(BPDLightVolume *volume) {
    if (maxLightPriority < 0 || volume->priority <= maxLightPriority)
        lightVolumes.AddVolume(volume);
}

void CStaticObject::ExitLightVolume(BPDLightVolume *volume) {
    if (maxLightPriority < 0 || volume->priority <= maxLightPriority)
        lightVolumes.RemoveVolume(volume);
}

BPDLightVolume *CStaticObject::GetLightVolume() { return lightVolumes.GetVolume(); }
