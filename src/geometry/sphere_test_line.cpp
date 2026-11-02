// Fragment of sphere.cpp: the line test solves the quadratic for the entry point, and a
// degenerate line is a point test at its end.
#include "Sphere.h"
#include "CLine3.h"

static inline CVector3 StartCopy(const CLine3 &line) { return CVector3(line.start); }
static inline CVector3 EndCopy(const CLine3 &line) { return CVector3(line.end); }
static inline CVector3 DirectionCopy(const CLine3 &line) { return CVector3(line.direction); }
static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline float DotOf(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}
static inline float SumSquares(const CVector3 &v) {
    float sum = v.x * v.x;
    sum += v.y * v.y;
    sum += v.z * v.z;
    return sum;
}
static inline void AddScaled(CVector3 &r, const CVector3 &a, const CVector3 &v, float s) {
    r.x = v.x * s + a.x;
    r.y = v.y * s + a.y;
    r.z = v.z * s + a.z;
}
static inline void Normalize(CVector3 &v) {
    float length = sqrtf(v.LengthSquared());
    if (length != 0.0f)
        v *= 1.0f / length;
}

EClsnState CVolSphere::TestCollision(const CLine3 &line, CCollision &collision, bool flag) const {
    const float epsilon = 1e-5f;
    CVector3 direction;
    direction = DirectionCopy(line);
    CVector3 diff;
    SetSub(diff, StartCopy(line), center);
    float t;
    float distSq;
    // Plain reference: reproduces the code shape.
    const CVector3 &dir = direction;
    float a = dir.x * dir.x + dir.y * dir.y;
    a += dir.z * dir.z;
    float dot = DotOf(direction, diff);
    float b = dot + dot;
    float c = SumSquares(diff) - radius * radius;

    if (!(a <= epsilon)) {
        float discriminant = b * b - a * (c * 4.0f);
        if (discriminant < 0.0f) {
            if (flag)
                collision.RaiseResult(1);
            return (EClsnState)1;
        }
        if (!(discriminant <= epsilon)) {
            float root = sqrtf(discriminant);
            float inv = 0.5f / a;
            float t0 = (-b - root) * inv;
            float t1 = (-b + root) * inv;
            if (t0 > 1.0f || t1 < 0.0f) {
                if (flag)
                    collision.RaiseResult(1);
                return (EClsnState)1;
            }
            if (!(t0 < 0.0f)) {
                t = t0;
            } else if (collision.lineTest) {
                t = 0.0f;
            } else {
                if (flag)
                    collision.RaiseResult(2);
                return (EClsnState)2;
            }
        } else {
            t = -b / (a + a);
        }
    } else {
        t = 1.0f;
        collision.depth = t;
        float separation = sqrtf(SumSquares(diff)) - radius;
        int code = 2;
        if (!(separation < 0.0f))
            code = 1;
        if (flag) {
            collision.RaiseResult(code);
            if (code == 2) {
                SClsnContact contact;
                contact.separation = separation;
                contact.position = EndCopy(line);
                contact.normal = diff;
                Normalize(contact.normal);
                contact.feature = 0;
                collision.AddContact(contact);
            }
        }
        return (EClsnState)code;
    }
    if (t < 0.0f || !(t <= 1.0f)) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    collision.depth = t;
    CVector3 point;
    AddScaled(point, StartCopy(line), direction, t);
    int result;
    if (collision.lineTest) {
        distSq = 0.0f;
        result = 2;
    } else {
        SetSub(diff, EndCopy(line), point);
        distSq = diff.LengthSquared();
        result = 2;
    }
    if (flag) {
        collision.RaiseResult(result);
        if (result == 2) {
            CVector3 normal;
            SetSub(normal, point, center);
            Normalize(normal);
            SClsnContact contact;
            contact.separation = -sqrtf(distSq);
            contact.position = point;
            contact.normal = normal;
            contact.feature = 0;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)result;
}
