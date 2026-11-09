// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The
// header name is a project choice.
#ifndef GAME_BOX_H
#define GAME_BOX_H
#pragma interface
#include "Volume.h"

class CVolBox : public IVolume {
  public:
    void SetCorners(const CVector3 &, const CVector3 &);
    virtual ~CVolBox() {}
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
    virtual EClsnState TestCollision(const CCSGVolume &, CCollision &, bool) const;
    // The half sizes and the frame, as CCDBVolBox's constructor fills them; names descriptive.
    CVolBox() {}
    CVolBox &operator=(const CVolBox &); // Body in BoxSetters.h.
    // Bodies in BoxSetters.h.
    void SetBasis(const CVector3 &x, const CVector3 &y, const CVector3 &z);
    void SetWidth(float w);
    void SetDepth(float d);
    void SetHeight(float h);
    void SetCenter(const CVector3 &c);
    float halfSize[3];
    CVector3 axisX;
    CVector3 axisY;
    CVector3 axisZ;
    CVector3 center;
};

#endif
