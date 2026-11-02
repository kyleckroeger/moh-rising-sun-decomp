// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The
// header name is a project choice.
#ifndef GAME_WORLDVOLUME_H
#define GAME_WORLDVOLUME_H
#include "Volume.h"

class CWorldVolume : public IVolume {
  public:
    CWorldVolume(int);
    virtual ~CWorldVolume();
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
    virtual void GetExtents(CVector3 &, CVector3 &) const;
    virtual EClsnState TestCollision(const CCSGVolume &, CCollision &, bool) const;
    template <class T> EClsnState TestCollisionWithSubVolumes(const T &, CCollision &, bool) const;
    unsigned char unknown_04[44];
    CVector3 extentLow;
    CVector3 extentHigh;
};

#endif
