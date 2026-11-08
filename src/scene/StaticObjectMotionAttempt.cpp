// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original static_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"
#include "MathFun.h"

extern "C" float sqrtf(float);
float MathFunAtan2F(float, float);
float MathFunNormalizeAngleNegativePiToPi(float);
void MathFunRotateAboutZ(float *, float *, float);
unsigned int GetFrameCount();
static inline float fabsf(float x) { return __builtin_fabsf(x); }

static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}

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

// Turn about an axis in place, keeping the position.
static inline void RotateInPlace(CMatrix *m, const CVector3 &axis, float angle) {
    CMatrix rotation;
    CMatrix *r = &rotation;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    CVector3 pos = m->GetPos();
    m->SetPos(CVector3(0.0f, 0.0f, 0.0f));
    r->BuildRot(axis, angle);
    CMatrix::s_TempMat = *m;
    m->Multiply(*r, CMatrix::s_TempMat);
    m->SetPos(pos);
}

// The frame layout shows the axis is this inline's own local, placed before RotateInPlace's locals.
static inline void RotateX(CMatrix *m, float angle) {
    CVector3 axis(1.0f, 0.0f, 0.0f);
    RotateInPlace(m, axis, angle);
}

void CStaticObject::AttemptScriptMotionUpdate(float dt) {
    float moveStep = dt;
    float turnStep = dt;
    float scale = scriptScaleBase;
    CVector3 delta;
    delta.Zero();
    if (flags.scriptMoving) {
        if (!(dt <= scriptMoveTime))
            moveStep = scriptMoveTime;
        SetScaled(delta, velocity, moveStep);
    }
    if (flags.scriptRotating) {
        if (!(dt <= scriptRotateTime))
            turnStep = scriptRotateTime;
        if (!(turnStep >= 1.0f)) {
            CQuaternion identity;
            CQuaternion &q = identity;
            q.w = 1.0f;
            q.x = 0.0f;
            q.y = 0.0f;
            q.z = 0.0f;
            scriptTargetOrientation.Slerp(scriptRotationDelta, q, turnStep);
        } else {
            scriptTargetOrientation = scriptRotationDelta;
        }
        scriptTargetOrientation.Multiply(orientation);
    } else {
        scriptTargetOrientation = orientation;
    }
    if (flags.scriptScaling) {
        if (!(dt <= scriptScaleRate))
            dt = scriptScaleRate;
        scale += scriptScaleTarget * dt;
    }
    scriptTargetOrientation.GetMatrix(transform);
    transform.SetPos(pathPosition);
    Transform(delta, transform, delta);
    transform.SetPos(delta);
    CMatrix scaling;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    scaling.BuildScale(scale);
    CMatrix::s_TempMat = transform;
    transform.Multiply(scaling, CMatrix::s_TempMat);
    scriptMoveStep = moveStep;
    scriptRotateStep = turnStep;
    scriptScaleStep = dt;
    IVolume *world = (IVolume *)field_13c;
    if (world) {
        world->TransformedCopy(*worldVolume, transform);
        CVector3 min, max;
        ((IVolume *)field_13c)->GetExtents(min, max);
        sortMinX->value = min.x;
        sortMinY->value = min.y;
        sortMaxX->value = max.x;
        sortMaxY->value = max.y;
    } else {
        CVector3 position;
        ((ISceneNode *)this)->GetPosition(position);
        sortMinX->value = position.x;
        sortMinY->value = position.y;
        sortMaxX->value = position.x;
        sortMaxY->value = position.y;
    }
}

void CStaticObject::AttemptAnimMotionUpdate(float dt) {
    if (motionLoop != 0 && motionRate != 0.0f) {
        CVector3 delta;
        SetScaled(delta, velocity, dt);
        motionStep = dt;
        float t = dt * motionSampleRate;
        if (!(t <= 1.0f))
            t = 1.0f;
        else if (!(t >= 0.0f))
            t = 0.0f;
        CQuaternion rotation;
        rotation.Slerp(motionResetOrientation, animOrientation, t);
        rotation.GetMatrix(transform);
        transform.SetPos(animPosition);
        transform.Translate(delta);
    }
    IVolume *world = (IVolume *)field_13c;
    if (world) {
        world->TransformedCopy(*worldVolume, transform);
        CVector3 min, max;
        ((IVolume *)field_13c)->GetExtents(min, max);
        sortMinX->value = min.x;
        sortMinY->value = min.y;
        sortMaxX->value = max.x;
        sortMaxY->value = max.y;
    } else {
        CVector3 position;
        ((ISceneNode *)this)->GetPosition(position);
        sortMinX->value = position.x;
        sortMinY->value = position.y;
        sortMaxX->value = position.x;
        sortMaxY->value = position.y;
    }
}

void CStaticObject::AttemptPathMotionUpdate(float dt) {
    CVector3 dir;
    SetSub(dir, pathDest, pathPosition);
    float length = sqrtf(dir.LengthSquared());
    if (length != 0.0f)
        dir *= 1.0f / length;
    if (!flags.pathFlag1) {
        CVector3 right, v, up;
        v = transform.GetFront();
        float yaw = MathFunAtan2F(v.x, v.y);
        MathFunRotateAboutZ(&v.x, &v.y, -yaw);
        float pitch = MathFunAtan2F(-v.z, v.y);
        v = dir;
        float targetYaw = MathFunAtan2F(v.x, v.y);
        MathFunRotateAboutZ(&v.x, &v.y, -targetYaw);
        float targetPitch = MathFunAtan2F(-v.z, v.y);
        float turn = MathFunNormalizeAngleNegativePiToPi(targetYaw - yaw);
        if (!(turn >= -0.008726647f)) {
            if (!(pathYawRate <= 0.0f)) {
                if (!(fabsf(pathYawRate / pathTurnAcceleration) + 2.0f < fabsf(turn / pathYawRate))) {
                    pathYawRate -= pathTurnAcceleration;
                } else {
                    pathYawRate += pathTurnAcceleration;
                    if (!(pathYawRate <= pathMaxTurnRate))
                        pathYawRate = pathMaxTurnRate;
                }
            } else {
                pathYawRate += pathTurnAcceleration;
                if (!(pathYawRate <= pathMaxTurnRate))
                    pathYawRate = pathMaxTurnRate;
            }
        } else if (!(turn <= 0.008726647f)) {
            if (!(pathYawRate >= 0.0f)) {
                if (!(fabsf(pathYawRate / pathTurnAcceleration) + 2.0f < fabsf(turn / pathYawRate))) {
                    pathYawRate += pathTurnAcceleration;
                } else {
                    pathYawRate -= pathTurnAcceleration;
                    if (!(pathYawRate >= -pathMaxTurnRate))
                        pathYawRate = -pathMaxTurnRate;
                }
            } else {
                pathYawRate -= pathTurnAcceleration;
                if (!(pathYawRate >= -pathMaxTurnRate))
                    pathYawRate = -pathMaxTurnRate;
            }
        } else {
            pathYawRate = pathYawRate * 0.6f;
        }
        turn = MathFunNormalizeAngleNegativePiToPi(targetPitch - pitch);
        if (!(turn >= -0.008726647f)) {
            if (!(pathPitchRate <= 0.0f)) {
                if (!(fabsf(pathPitchRate / pathTurnAcceleration) + 2.0f < fabsf(turn / pathPitchRate))) {
                    pathPitchRate -= pathTurnAcceleration;
                } else {
                    pathPitchRate += pathTurnAcceleration;
                    if (!(pathPitchRate <= pathMaxTurnRate))
                        pathPitchRate = pathMaxTurnRate;
                }
            } else {
                pathPitchRate += pathTurnAcceleration;
                if (!(pathPitchRate <= pathMaxTurnRate))
                    pathPitchRate = pathMaxTurnRate;
            }
        } else if (!(turn <= 0.008726647f)) {
            if (!(pathPitchRate >= 0.0f)) {
                if (!(fabsf(pathPitchRate / pathTurnAcceleration) + 2.0f < fabsf(turn / pathPitchRate))) {
                    pathPitchRate += pathTurnAcceleration;
                } else {
                    pathPitchRate -= pathTurnAcceleration;
                    if (!(pathPitchRate >= -pathMaxTurnRate))
                        pathPitchRate = -pathMaxTurnRate;
                }
            } else {
                pathPitchRate -= pathTurnAcceleration;
                if (!(pathPitchRate >= -pathMaxTurnRate))
                    pathPitchRate = -pathMaxTurnRate;
            }
        } else {
            pathPitchRate = pathPitchRate * 0.6f;
        }
        RotateX(&transform, -pathPitchRate);
        right = transform.GetRight();
        v = transform.GetFront();
        up = transform.GetUp();
        MathFunRotateAboutZ(&right.x, &right.y, -pathYawRate);
        MathFunRotateAboutZ(&v.x, &v.y, -pathYawRate);
        MathFunRotateAboutZ(&up.x, &up.y, -pathYawRate);
        transform.SetRight(right);
        transform.SetFront(v);
        transform.SetUp(up);
        if (GetFrameCount() & 0x20)
            transform.Orthonormalize();
    }
    float speed;
    if (pathTurnSlowdown != 0.0f)
        speed = pathMaxSpeed * (1.0f - pathTurnSlowdown / pathMaxTurnRate * fabsf(pathYawRate));
    else
        speed = pathMaxSpeed;
    speed = MathFunRandomReal(0.8f * speed, speed);
    if (!(pathSpeed <= speed)) {
        pathSpeed = pathPreviousSpeed - 2.0f * pathAcceleration * dt;
        if (!(pathSpeed >= speed))
            pathSpeed = speed;
    } else {
        pathSpeed = pathAcceleration * dt + pathPreviousSpeed;
        if (!(pathSpeed <= speed))
            pathSpeed = speed;
    }
    if (flags.pathFlag1) {
        dir *= pathSpeed * dt;
    } else {
        float step = pathSpeed * dt;
        CVector3 front = transform.GetFront();
        SetScaled(dir, front, step);
    }
    dir += pathPosition;
    transform.SetPos(dir);
    IVolume *world = (IVolume *)field_13c;
    if (world) {
        CVector3 min, max;
        world->TransformedCopy(*worldVolume, transform);
        ((IVolume *)field_13c)->GetExtents(min, max);
        sortMinX->value = min.x;
        sortMinY->value = min.y;
        sortMaxX->value = max.x;
        sortMaxY->value = max.y;
    } else {
        CVector3 position;
        ((ISceneNode *)this)->GetPosition(position);
        sortMinX->value = position.x;
        sortMinY->value = position.y;
        sortMaxX->value = position.x;
        sortMaxY->value = position.y;
    }
}
