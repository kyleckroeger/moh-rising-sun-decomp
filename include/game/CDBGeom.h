// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The
// header name is a project choice.
// The table keeps CVolBox entries for Create, TransformedCopy, visibility and
// extents, so CCDBVolBox derives from CVolBox.
#ifndef GAME_CDBGEOM_H
#define GAME_CDBGEOM_H
#pragma interface
#include "Box.h"

class CDBBox;
struct CDBTri;

class CCDBVolBox : public CVolBox {
  public:
    CCDBVolBox(const CDBBox &);
    virtual ~CCDBVolBox() {}
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
};

class CCDBTriangle {
  public:
    EClsnState TestCollision(const CVolSphere &, CCollision &, const CDBTri *, bool) const;
    EClsnState TestCollision(const CAnimatedVolume &, CCollision &, const CDBTri *, bool) const;
};

#endif
