// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

extern "C" float sqrtf(float);

static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) {
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
}

static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}

static inline void SetLerp(CVector3 &r, const CVector3 &a, const CVector3 &b, float t) {
    float fx = a.x, fy = a.y, fz = a.z;
    r.x = (b.x - fx) * t + fx;
    r.y = (b.y - fy) * t + fy;
    r.z = (b.z - fz) * t + fz;
}

static inline CVector3 KeyPosition(const MotionFrame *frames, int key) { return frames[key].position; }
static inline CQuaternion KeyRotation(const MotionFrame *frames, int key) { return frames[key].rotation; }

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

void CHierObject::BeginAnimMotionUpdate(float dt) {
    if (motionLoop != 0 && motionRate != 0.0f) {
        animPosition = attachOffset;
        animOrientation = orientation;
        motionKeyTime += dt;
        float rate = motionRate;
        if (!(motionKeyTime < rate)) {
            do {
                motionKeyTime -= rate;
                motionTimer++;
            } while (motionKeyTime >= rate);
        }
        int next = motionTimer + 1;
        if (motionKey) {
            if (next >= motionKey)
                next = motionKey - 1;
            if (motionTimer >= motionKey) {
                if (motionPlayEvent != -1) {
                    TriggerScriptEvent((unsigned short)motionPlayEvent, 0, 0);
                    motionPlayEvent = -1;
                }
                flags.motionPlaying = 0;
                dumpAge = 0.0f;
                motionTimer = motionKey - 1;
            }
        } else {
            motionTimer %= motionLoop;
            next %= motionLoop;
        }
        CVector3 from = KeyPosition(motionFrames, motionTimer);
        CVector3 to = KeyPosition(motionFrames, next);
        CVector3 pos;
        if (motionSpeedKey != motionTimer) {
            CVector3 delta;
            SetSub(delta, to, from);
            dumpAge = sqrtf(delta.LengthSquared()) * (60.0f / motionRate);
            motionSpeedKey = motionTimer;
        }
        float t = motionKeyTime / motionRate;
        if (!(t >= 0.0f))
            t = 0.0f;
        if (!(t <= 1.0f))
            t = 1.0f;
        SetLerp(pos, from, to, t);
        CQuaternion current = KeyRotation(motionFrames, motionTimer);
        CQuaternion target = KeyRotation(motionFrames, next);
        CQuaternion rotation;
        rotation.Slerp(target, current, t);
        if (flags.hasMotion) {
            motionStartOrientation.Rotate(pos, pos);
            pos += motionStartPosition;
            rotation.Multiply(motionStartOrientation);
        }
        SetSub(velocity, pos, animPosition);
        float scale = 1.0f / dt;
        motionResetOrientation = rotation;
        velocity *= scale;
        motionSampleRate = scale;
    }
}

void CHierObject::AttemptAnimMotionUpdate(float dt) {
    if (motionLoop != 0 && motionRate != 0.0f) {
        SetScaled(attachDelta, velocity, dt);
        CQuaternion rotation;
        rotation.Slerp(motionResetOrientation, animOrientation, dt * motionSampleRate);
        scriptTargetOrientation = rotation;
        rotation.GetMatrix(transform);
        if (flags.hasMotion)
            Transform(attachDelta, transform, attachDelta);
        transform.SetPos(animPosition);
        transform.Translate(attachDelta);
        PostMultiply(transform, attachPointMatrix);
        PostMultiply(transform, inheritedMatrix);
    }
    if (field_13c && worldVolume)
        ((IVolume *)field_13c)->TransformedCopy(*worldVolume, transform);
}
