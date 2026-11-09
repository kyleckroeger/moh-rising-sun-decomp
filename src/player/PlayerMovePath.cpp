// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "AISplinePath.h"
extern "C" float sqrtf(float);
void EvaluateSplinePath(CAISplinePath *, float, CVector3 &);
void EvaluateSplineTangent(CAISplinePath *, float, CVector3 &);
static inline void Normalize(CVector3 &v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length != 0.0f) {
        float scale = 1.0f / length;
        v.x *= scale;
        v.y *= scale;
        v.z *= scale;
    }
}
static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) {
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
}
static inline void SetSum(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
}
static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline void SetCross(CVector3 &out, const CVector3 &a, const CVector3 &b) {
    CVector3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    out.x = r.x;
    out.y = r.y;
    out.z = r.z;
}
void CPlayerObject::UpdateMovePath(float dt) {
    if (pathFlags & 0x80000) {
        float t = pathParameter + pathRate * (dt * 0.5f);
        pathParameter += pathRate * dt;
        if (!(t >= 0.0f)) t = 0.0f;
        if (!(t <= 1.0f)) {
            pathFlags &= ~0x80000;
            t = 1.0f;
        }
        CVector3 target;
        if (movePath) EvaluateSplinePath(movePath, t, target);
        else {
            CVector3 step;
            SetScaled(step, velocity, dt);
            SetSum(target, position, step);
        }
        SetSub(velocity, target, position);
        velocity *= 1.0f / dt;
        CVector3 front;
        if (lookPath && lookPath == movePath) {
            EvaluateSplineTangent(lookPath, t, front);
            Normalize(front);
        } else if (lookPath) {
            CVector3 next;
            EvaluateSplinePath(lookPath, t, next);
            SetSub(front, next, target);
            Normalize(front);
        } else {
            return;
        }
        CVector3 up(0.0f, 0.0f, 1.0f);
        CVector3 right;
        SetCross(right, front, up);
        Normalize(right);
        SetCross(up, right, front);
        Normalize(up);
        if (!CMatrix::s_ClassInit) CMatrix::InitClass();
        CMatrix matrix;
        float unused;
        matrix.SetRight(right);
        matrix.SetUp(up);
        matrix.SetFront(front);
        matrix.ToEulerXYZ(pitch, unused, yaw);
    }
}
