// Fragment of capsule.cpp: the local FindIntersection and the line test that calls it.
#include "Capsule.h"
#include "Sphere.h"
#include <math.h>
extern "C" float sqrtf(float);

static inline CVector3 StartCopy(const CLine3 &line) { return CVector3(line.start); }
static inline CVector3 EndCopy(const CLine3 &line) { return CVector3(line.end); }
static inline CVector3 DirectionCopy(const CLine3 &line) { return CVector3(line.direction); }
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
static inline float NormalizeOf(CVector3 &v) {
    float length = sqrtf(v.LengthSquared());
    if (length != 0.0f)
        v *= 1.0f / length;
    return length;
}
static inline float DotOf(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}

// The structure follows the standard quadratic for a line against a capsule: the infinite
// cylinder, then each end's hemisphere. Local names are descriptive.
static int FindIntersection(const CLine3 &line, const CVolCapsule &capsule, float *t) {
    CVector3 origin = StartCopy(line);
    CVector3 lineDirection = DirectionCopy(line);
    CVector3 u;
    CVector3 v;
    CVector3 w = DirectionCopy(capsule.line);
    float wLength = NormalizeOf(w);
    if (!((float)fabs(w.x) < (float)fabs(w.y))) {
        float inv = 1.0f / sqrtf(w.x * w.x + w.z * w.z);
        u.x = -w.z * inv;
        u.y = 0.0f;
        u.z = w.x * inv;
    } else {
        float inv = 1.0f / sqrtf(w.y * w.y + w.z * w.z);
        u.x = 0.0f;
        u.y = w.z * inv;
        u.z = -w.y * inv;
    }
    v.x = w.y * u.z - w.z * u.y;
    v.y = w.z * u.x - w.x * u.z;
    v.z = w.x * u.y - w.y * u.x;
    CVector3 d;
    d.x = DotOf(u, lineDirection);
    d.y = DotOf(v, lineDirection);
    d.z = DotOf(w, lineDirection);
    float dLength = NormalizeOf(d);
    float invDLength = 1.0f / dLength;
    CVector3 diff;
    SetSub(diff, origin, StartCopy(capsule.line));
    CVector3 p;
    p.x = DotOf(u, diff);
    p.y = DotOf(v, diff);
    p.z = DotOf(w, diff);
    float radiusSquared = capsule.GetRadius();
    radiusSquared *= radiusSquared;
    float a, b, c, discriminant, root, r, height, inv;
    const float epsilon = 1e-5f;
    if (!((float)fabs(d.z) < 1.0f - epsilon)) {
        float axial = radiusSquared - (p.x * p.x + p.y * p.y);
        if (axial < 0.0f)
            return 0;
        root = sqrtf(axial);
        t[0] = -(p.z + root) * invDLength;
        t[1] = (wLength - p.z + root) * invDLength;
        return 2;
    }
    a = d.x * d.x + d.y * d.y;
    b = p.x * d.x + p.y * d.y;
    c = p.x * p.x + p.y * p.y - radiusSquared;
    discriminant = b * b - a * c;
    if (!(discriminant >= 0.0f))
        return 0;
    int count = 0;
    if (!(discriminant <= epsilon)) {
        root = sqrtf(discriminant);
        inv = 1.0f / a;
        r = (-b - root) * inv;
        height = r * d.z + p.z;
        if (!(height < 0.0f) && !(height > wLength))
            t[count++] = r * invDLength;
        r = (-b + root) * inv;
        height = r * d.z + p.z;
        if (!(height < 0.0f) && !(height > wLength))
            t[count++] = r * invDLength;
        if (count == 2)
            return 2;
    } else {
        r = -b / a;
        height = r * d.z + p.z;
        if (!(height < 0.0f) && !(height > wLength)) {
            t[0] = r * invDLength;
            return 1;
        }
    }
    c += p.z * p.z;
    b += p.z * d.z;
    discriminant = b * b - c;
    if (!(discriminant <= epsilon)) {
        root = sqrtf(discriminant);
        r = -b - root;
        height = r * d.z + p.z;
        if (!(height > 0.0f)) {
            t[count++] = r * invDLength;
            if (count == 2)
                return 2;
        }
        r = -b + root;
        height = r * d.z + p.z;
        if (!(height > 0.0f)) {
            t[count++] = r * invDLength;
            if (count == 2)
                return 2;
        }
    } else if (!(discriminant < 0.0f)) {
        r = -b;
        height = r * d.z + p.z;
        if (!(height > 0.0f)) {
            t[count++] = r * invDLength;
            if (count == 2)
                return 2;
        }
    }
    b -= d.z * wLength;
    c += wLength * (wLength - (p.z + p.z));
    discriminant = b * b - c;
    if (!(discriminant <= epsilon)) {
        root = sqrtf(discriminant);
        r = -b - root;
        height = r * d.z + p.z;
        if (!(height < wLength)) {
            t[count++] = r * invDLength;
            if (count == 2)
                return 2;
        }
        r = -b + root;
        height = r * d.z + p.z;
        if (!(height < wLength)) {
            t[count++] = r * invDLength;
            if (count == 2)
                return 2;
        }
    } else if (!(discriminant < 0.0f)) {
        r = -b;
        height = r * d.z + p.z;
        if (!(height < wLength)) {
            t[count++] = r * invDLength;
            if (count == 2)
                return 2;
        }
    }
    return count;
}

EClsnState CVolCapsule::TestCollision(const CLine3 &other, CCollision &collision, bool flag) const {
    if (line.IsDegenerate()) {
        CVolSphere sphere(radius, StartCopy(line));
        return sphere.CVolSphere::TestCollision(other, collision, flag);
    }
    float roots[2];
    float t = 0.0f;
    switch (FindIntersection(other, *this, roots)) {
    case 0:
        t = -1.0f;
        break;
    case 1:
        t = roots[0];
        break;
    case 2:
        if (!(roots[0] >= roots[1]) && !(roots[0] < t))
            t = roots[0];
        else
            t = roots[1];
        break;
    }
    if (t < 0.0f || !(t <= 1.0f)) {
        if (flag)
            collision.RaiseResult(1);
        return (EClsnState)1;
    }
    collision.depth = t;
    CVector3 point;
    SetScaled(point, DirectionCopy(other), t);
    point += StartCopy(other);
    CVector3 delta;
    float squared;
    int result;
    if (collision.lineTest) {
        squared = 0.0f;
        result = 2;
    } else {
        SetSub(delta, point, EndCopy(other));
        squared = delta.LengthSquared();
        result = 2;
    }
    if (flag) {
        collision.RaiseResult(result);
        if (result == 2) {
            CVector3 nearest;
            float s;
            line.GetProjection(point, nearest, s);
            if (!(s >= 0.0f))
                nearest = StartCopy(line);
            else if (!(s <= 1.0f))
                nearest = EndCopy(line);
            SetSub(delta, point, nearest);
            float length = sqrtf(delta.LengthSquared());
            if (length != 0.0f)
                delta *= 1.0f / length;
            SClsnContact contact;
            contact.separation = -sqrtf(squared);
            contact.normal = delta;
            contact.position = point;
            contact.feature = 0;
            contact.subVolume = 0;
            collision.AddContact(contact);
        }
    }
    return (EClsnState)result;
}
