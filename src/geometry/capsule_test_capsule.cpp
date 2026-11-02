// Fragment of capsule.cpp: past either end of this capsule's line the test uses the
// projections' distance; otherwise the distance between the two segments.
#include "Capsule.h"
extern "C" float sqrtf(float);

static inline CVector3 StartCopy(const CLine3 &line) { return CVector3(line.start); }
static inline CVector3 EndCopy(const CLine3 &line) { return CVector3(line.end); }
static inline CVector3 DirectionCopy(const CLine3 &line) { return CVector3(line.direction); }
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
static inline void SetScaled(CVector3 &r, const CVector3 &a, float s) {
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
}

EClsnState CVolCapsule::TestCollision(const CVolCapsule &other, CCollision &collision, bool flag) const {
    float otherRadius = other.GetRadius();
    // Plain reference: reproduces the code shape.
    const CLine3 &source = other.line;
    CLine3 otherLine = source;
    float radiiSq = radius + otherRadius;
    radiiSq *= radiiSq;
    int alongAxis = 1;
    int pastEnd = 0;
    CVector3 p0;
    CVector3 p1;
    float t0;
    float t1;
    line.GetProjection(StartCopy(otherLine), p0, t0);
    line.GetProjection(EndCopy(otherLine), p1, t1);
    float s;
    float t;
    float distSq;
    line.GetSquaredDistanceBetweenSegments(other.line, s, t, &distSq);
    float length = line.CachedLength();
    if (!(distSq > radiiSq)) {
        float separation;
        if (!(t0 <= 1.0f) && !(t1 <= 1.0f)) {
            float a = (t0 - 1.0f) * length;
            float b = (t1 - 1.0f) * length;
            separation = a;
            if (!(a <= b))
                separation = b;
            pastEnd = 1;
        } else if (!(t0 >= 0.0f) && !(t1 >= 0.0f)) {
            float a = -t0 * length;
            float b = -t1 * length;
            separation = a;
            if (!(a <= b))
                separation = b;
        } else {
            alongAxis = 0;
            float radius2 = other.GetRadius();
            separation = sqrtf(distSq) - (radius + radius2);
        }
        int result = 2;
        if (!(separation < 0.0f))
            result = 1;
        if (flag && result != 1) {
            CVector3 normal;
            CVector3 point;
            if (alongAxis) {
                normal = DirectionCopy(line);
                if (pastEnd) {
                    normal *= -1.0f;
                    point = StartCopy(line);
                } else {
                    point = EndCopy(line);
                }
            } else {
                normal = StartCopy(otherLine);
                normal -= p0;
                float len = sqrtf(normal.LengthSquared());
                if (len != 0.0f)
                    normal *= 1.0f / len;
                SetScaled(point, normal, radius);
                point += p0;
            }
            collision.RaiseResult(result);
            if (result == 2) {
                SClsnContact contact;
                contact.separation = separation;
                contact.position = point;
                contact.normal = normal;
                contact.feature = 0;
                contact.subVolume = 0;
                collision.AddContact(contact);
            }
        }
        return (EClsnState)result;
    }
    return (EClsnState)1;
}
