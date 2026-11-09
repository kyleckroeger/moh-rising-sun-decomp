// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) {
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
}

static inline void Transform(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}

static inline void PostMultiply(CMatrix &m, const CMatrix &o) {
    CMatrix::s_TempMat = m;
    m.Multiply(CMatrix::s_TempMat, o);
}

void CHierObject::AttemptScriptMotionUpdate(float dt) {
    float moveStep = dt;
    if (flags.scriptRotating) {
        if (!(dt <= scriptRotateTime))
            dt = scriptRotateTime;
        if (!(dt >= 1.0f)) {
            CQuaternion identity;
            CQuaternion &q = identity;
            q.w = 1.0f;
            q.x = 0.0f;
            q.y = 0.0f;
            q.z = 0.0f;
            scriptTargetOrientation.Slerp(scriptRotationDelta, q, dt);
        } else {
            scriptTargetOrientation = scriptRotationDelta;
        }
        scriptTargetOrientation.Multiply(orientation);
    } else {
        scriptTargetOrientation = orientation;
    }
    scriptTargetOrientation.GetMatrix(transform);
    if (flags.scriptMoving) {
        if (!(moveStep <= scriptMoveTime))
            moveStep = scriptMoveTime;
        SetScaled(attachDelta, velocity, moveStep);
        Transform(attachDelta, transform, attachDelta);
    } else {
        attachDelta.Zero();
    }
    transform.SetPos(attachDelta);
    transform.Translate(attachOffset);
    PostMultiply(transform, attachPointMatrix);
    PostMultiply(transform, inheritedMatrix);
    scriptMoveStep = moveStep;
    scriptRotateStep = dt;
    if (field_13c && worldVolume)
        ((IVolume *)field_13c)->TransformedCopy(*worldVolume, transform);
}
