// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ObserverTypes.h"
#include "EAGLTransform.h"

float CAnimObject::GetYaw() const { return yaw; }

void CAnimObject::SetYaw(float angle) {
    float negativePi = -3.14159274f, pi = 3.14159274f;
    if (!(angle <= pi)) {
        do {
            angle -= 6.28318548f;
        } while (angle > pi);
    }
    if (!(angle >= negativePi)) {
        do {
            angle += 6.28318548f;
        } while (angle < negativePi);
    }
    yaw = angle;
    yawCopy = angle;
}

void CAnimObject::Orthonormalize() { transform.Orthonormalize(); }
void CAnimObject::GetPosition(CVector3 &out) const { out = transform.GetPos(); }
void CAnimObject::GetRightward(CVector3 &out) const { out = transform.GetRight(); }
void CAnimObject::GetForward(CVector3 &out) const { out = transform.GetFront(); }
void CAnimObject::GetUpward(CVector3 &out) const { out = transform.GetUp(); }
void CAnimObject::EnterLightVolume(BPDLightVolume *volume) { lightVolumes.AddVolume(volume); }
void CAnimObject::ExitLightVolume(BPDLightVolume *volume) { lightVolumes.RemoveVolume(volume); }
BPDLightVolume *CAnimObject::GetLightVolume() { return lightVolumes.GetVolume(); }
void CAnimObject::SetTMLocalToWorld(const CMatrix &matrix) { transform = matrix; }

void CAnimObject::AttachToLocation(const CMatrix &location, float blend) {
    attached = 1;
    attachedToLocation = 1;
    attachBlend = blend;
    attachLocation = location;
}
