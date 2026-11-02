// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The
// header name is a project choice.
// The table keeps CVolSphere's visibility, extents and radius entries, and
// TestCollision(const CAnimatedVolume &) on a sphere forwards to the sphere test
// without an adjustment: a CVolSphere base at offset zero.
#ifndef GAME_ANIMATEDVOLUME_H
#define GAME_ANIMATEDVOLUME_H
#include "Sphere.h"

class CAnimatedVolume : public CVolSphere {
  public:
    CAnimatedVolume();
    virtual ~CAnimatedVolume();
    virtual IVolume *Create() const;
    virtual void TransformedCopy(const IVolume &, const CMatrix &);
    virtual EClsnState TestCollision(const IVolume &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolBox &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolSphere &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolCapsule &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CCDBObject &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVolFiber &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CAnimatedVolume &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CWorldVolume &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CVector3 &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CLine3 &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CPlane &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CTriangle &, CCollision &, bool) const;
    virtual EClsnState TestCollision(const CCSGVolume &, CCollision &, bool) const;
    int unknown_20;
    int unknown_24;
    unsigned char unknown_28[8];
    int unknown_30;
};

#endif
