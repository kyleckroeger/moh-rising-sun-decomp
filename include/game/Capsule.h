// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The
// header name is a project choice.
#ifndef GAME_CAPSULE_H
#define GAME_CAPSULE_H
#include "Volume.h"
#include "CLine3.h"

class CVolCapsule : public IVolume {
  public:
    CVolCapsule();
    virtual ~CVolCapsule();
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
    virtual int TestVisibility(CDrawContext &) const;
    virtual void GetExtents(CVector3 &, CVector3 &) const;
    virtual float GetRadius() const;
    virtual void SetRadius(float);
    virtual EClsnState TestCollision(const CCSGVolume &, CCollision &, bool) const;
    unsigned char unknown_04[8];
    float radius;
    CLine3 line;
};

inline CVolCapsule::CVolCapsule() {}

#endif
