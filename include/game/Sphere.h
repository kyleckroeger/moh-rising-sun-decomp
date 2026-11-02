// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The
// header name is a project choice.
// 32 bytes (CVolSphere::Create allocates 32). The radius at +12 and center at
// +16 are established by GetRadius/SetRadius and GetExtents; +4 is unknown.
#ifndef GAME_SPHERE_H
#define GAME_SPHERE_H
#pragma interface
#include "Volume.h"

class CVolSphere : public IVolume {
  public:
    virtual ~CVolSphere() {}
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
    virtual void SetRadius(float r) { radius = r; }
    virtual float GetRadius() const { return radius; }
    virtual EClsnState TestCollision(const CCSGVolume &, CCollision &, bool) const;
    unsigned char unknown_04[8];
    float radius;
    CVector3 center;
    CVolSphere() {}
    CVector3 GetCenter() const { return center; }
    CVolSphere(float r, const CVector3 &c) : radius(r), center(c) {}
};

#endif
