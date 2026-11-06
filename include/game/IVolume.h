// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The header
// name is a project choice. Every IVolume virtual is inline: the original emits them
// together in table order, as one group outside any .cpp's own methods.
#ifndef GAME_IVOLUME_H
#define GAME_IVOLUME_H
#pragma interface
#include "CVector3.h"

class CCollision;
class CDrawContext;
class CMatrix;
class CLine3;
class CPlane;
class CTriangle;
class CVolBox;
class CVolSphere;
class CVolCapsule;
class CVolFiber;
class CCDBObject;
class CAnimatedVolume;
class CWorldVolume;
class CCSGVolume;
class CHierObject;
class CDB;

// The name is a return type recorded in the template symbols
// TestCollisionWithSubVolumes__H1Z...; its values are not recovered by name: the
// sphere tests return 1 when apart and 2 on contact.
enum EClsnState {};

// The virtual functions are declared in the order of _vt.7IVolume, whose entries
// name each one. The mangled names do not record return types: EClsnState for the
// tests is taken from the template symbols that dispatch them.
class IVolume {
  public:
    virtual ~IVolume() {}
    virtual IVolume *Create() const { return 0; }
    virtual void TransformedCopy(const IVolume &, const CMatrix &) {}
    virtual EClsnState TestCollision(const IVolume &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CVolBox &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CVolSphere &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CVolCapsule &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CCDBObject &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CVolFiber &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CAnimatedVolume &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CWorldVolume &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CVector3 &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CLine3 &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CPlane &, CCollision &, bool) const { return (EClsnState)0; }
    virtual EClsnState TestCollision(const CTriangle &, CCollision &, bool) const { return (EClsnState)0; }
    virtual int TestVisibility(CDrawContext &) const { return 0; }
    virtual void GetExtents(CVector3 &, CVector3 &) const {}
    virtual float GetRadius() const { return -1.0f; }
    virtual void SetRadius(float) {}
};

#endif
