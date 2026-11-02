#ifndef GAME_CLINE3_H
#define GAME_CLINE3_H

// AI-assisted storage view; see docs/Geometry.md. This observed 64-byte prefix
// does not establish the full historical class or allocation size.
#include "CVector3.h"

// Descriptive inline helper; no original function name or boundary is claimed.
static inline float LineVectorSquared(const CVector3& v) {
    float sum = v.x * v.x + v.y * v.y;
    sum += v.z * v.z;
    return sum;
}

class CLine3 {
public:
    CVector3 start, end, direction;
    mutable float length, lengthSquared;
    mutable int lengthValid, lengthSquaredValid;

    // Local name for the cache behavior inlined into the original callers.
    float CachedLengthSquared() const {
        if (!lengthSquaredValid) {
            lengthSquared = LineVectorSquared(direction);
            lengthSquaredValid = 1;
        }
        return lengthSquared;
    }

    CLine3() {}
    CLine3(const CVector3 &a, const CVector3 &b) { Set(a, b); }
    CLine3(const CLine3 &o)
        : start(o.start), end(o.end), direction(o.direction), length(o.length), lengthSquared(o.lengthSquared),
          lengthValid(o.lengthValid), lengthSquaredValid(o.lengthSquaredValid) {}
    // Inline assignment; the original copies guard against self-assignment.
    CLine3 &operator=(const CLine3 &o) {
        if (&o != this) {
            start = o.start;
            end = o.end;
            direction = o.direction;
            length = o.length;
            lengthSquared = o.lengthSquared;
            lengthValid = o.lengthValid;
            lengthSquaredValid = o.lengthSquaredValid;
        }
        return *this;
    }
    const CVector3 &GetStart() const { return start; }
    const CVector3 &GetEnd() const { return end; }

    float GetLengthSquared() const;
    int IsDegenerate() const;
    void Set(const CVector3&, const CVector3&);
    void GetProjection(const CVector3&, CVector3&, float&) const;
};

#endif
