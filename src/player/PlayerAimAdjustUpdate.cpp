// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "Camera.h"
extern "C" float sqrtf(float);
extern "C" float atan2f(float, float);
extern "C" float asinf(float);
static inline float NormalizeV(CVector3 &v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length != 0.0f) {
        float inverse = 1.0f / length;
        v.x *= inverse;
        v.y *= inverse;
        v.z *= inverse;
    }
    return length;
}
static inline float NormalizeXY(CVector3 &v) {
    v.z = 0.0f;
    float length = sqrtf(v.x * v.x + v.y * v.y);
    if (length != 0.0f) {
        float inverse = 1.0f / length;
        v.x *= inverse;
        v.y *= inverse;
        v.z *= inverse;
    }
    return length;
}
static inline float LengthSquared(const CVector3 &v) {
    float sum = v.x * v.x + v.y * v.y;
    sum += v.z * v.z;
    return sum;
}
static inline float Dot(const CVector3 &a, const CVector3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
void CPlayerObject::AimAdjust::UpdatePlayer(float &yaw, float &pitch, const CCamera &camera) {
    if (!active) return;
    CVector3 direction = target;
    CVector3 eye;
    camera.GetPosition(eye);
    direction -= eye;
    if (LengthSquared(direction) < 25.0f) return;
    CVector3 look;
    camera.GetUpward(look);
    NormalizeV(direction);
    float dot = Dot(look, direction);
    NormalizeXY(direction);
    CVector3 front;
    camera.GetForward(front);
    CVector3 right;
    camera.GetRightward(right);
    NormalizeXY(front);
    NormalizeXY(right);
    float ahead = Dot(direction, front);
    float side = Dot(direction, right);
    yaw = atan2f(side, ahead);
    pitch = asinf(dot);
}
