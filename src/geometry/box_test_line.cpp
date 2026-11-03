// Fragment of box.cpp: the line test clips the line against the three axis slabs.
#include "Box.h"
#include "CLine3.h"
#include <math.h>

static inline CVector3 StartCopy(const CLine3 &line) { return CVector3(line.start); }
static inline CVector3 DirectionCopy(const CLine3 &line) { return CVector3(line.direction); }
static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline void SetNegated(CVector3 &r, const CVector3 &a) {
    r.x = -a.x;
    r.y = -a.y;
    r.z = -a.z;
}
static inline void SetScaled(CVector3 &r, const CVector3 &a, float s) {
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
}
static inline void AddScaled(CVector3 &r, const CVector3 &a, const CVector3 &v, float s) {
    r.x = v.x * s + a.x;
    r.y = v.y * s + a.y;
    r.z = v.z * s + a.z;
}
static inline float DotOf(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}

static inline bool Clip(const CVector3 &axis, const CVector3 &offset, const CVector3 &dir, float half, float &tNear, float &tFar,
                        CVector3 &nearNormal, CVector3 &farNormal) {
    float d = DotOf(dir, axis);
    float e = DotOf(offset, axis);
    if (!((float)fabs(d) <= 1e-6f)) {
        float inv = 1.0f / d;
        float t2 = (-half - e) * inv;
        float t1 = (half - e) * inv;
        if (!(t2 >= t1)) {
            if (!(t2 <= tNear)) {
                tNear = t2;
                SetNegated(nearNormal, axis);
            }
            if (!(t1 >= tFar)) {
                tFar = t1;
                farNormal = axis;
            }
        } else {
            if (!(t1 <= tNear)) {
                tNear = t1;
                nearNormal = axis;
            }
            if (!(t2 >= tFar)) {
                tFar = t2;
                SetNegated(farNormal, axis);
            }
        }
    } else if (!((float)fabs(e) <= half)) {
        return false;
    }
    bool inside = false;
    if (!(tNear > tFar))
        inside = tFar >= 0.0f;
    return inside;
}

EClsnState CVolBox::TestCollision(const CLine3 &line, CCollision &collision, bool flag) const {
    CVector3 nearNormal;
    CVector3 farNormal;
    CVector3 offset;
    CVector3 dir;
    float tFar;
    float tNear = -3.4028235e+38f;
    tFar = 3.4028235e+38f;
    SetSub(offset, StartCopy(line), center);
    dir = DirectionCopy(line);


    if (!Clip(axisX, offset, dir, halfSize[0], tNear, tFar, nearNormal, farNormal) ||
        !Clip(axisY, offset, dir, halfSize[1], tNear, tFar, nearNormal, farNormal) ||
        !Clip(axisZ, offset, dir, halfSize[2], tNear, tFar, nearNormal, farNormal)) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    CVector3 normal;
    if (tNear > 1.0f || !(tFar >= 0.0f)) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    float t;
    if (!(tNear < 0.0f)) {
        normal = nearNormal;
        t = tNear;
    } else {
        normal = farNormal;
        t = 0.0f;
    }
    collision.depth = t;
    float distSq;
    int result;
    if (collision.lineTest) {
        distSq = 0.0f;
        result = 2;
    } else {
        CVector3 rest;
        SetScaled(rest, dir, 1.0f - t);
        distSq = rest.LengthSquared();
        result = 2;
    }
    if (flag) {
        collision.RaiseResult(result);
        if (result == 2) {
            CVector3 point;
            AddScaled(point, StartCopy(line), dir, t);
            SClsnContact contact;
            contact.separation = sqrtf(distSq);
            contact.position = point;
            contact.normal = normal;
            contact.feature = 0;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)result;
}
