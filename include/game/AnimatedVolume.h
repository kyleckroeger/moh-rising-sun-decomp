// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The
// header name is a project choice.
// The table keeps CVolSphere's visibility, extents and radius entries, and
// TestCollision(const CAnimatedVolume &) on a sphere forwards to the sphere test
// without an adjustment: a CVolSphere base at offset zero.
#ifndef GAME_ANIMATEDVOLUME_H
#define GAME_ANIMATEDVOLUME_H
#include "Sphere.h"

namespace EALA {
namespace Character {
struct CollisionData;
}
}

void *AllocateAnimatedVolume();
void FreeAnimatedVolume(void *);
class CVolCapsule;

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
    void *operator new(unsigned int) { return AllocateAnimatedVolume(); }
    void operator delete(void *p) { FreeAnimatedVolume(p); }
    void Set(EALA::Character::CollisionData *);
    int collisionFlag;
    EALA::Character::CollisionData *data;
    CVolCapsule *subVolumes;
    int unknown_2c;
    int unknown_30;
};

#endif
