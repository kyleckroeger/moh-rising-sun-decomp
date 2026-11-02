// AI-assisted scoped reconstruction from GR8E69; see docs/LightVolumes.md and
// docs/Collision.md.
#ifndef GAME_VOLUME_H
#define GAME_VOLUME_H
#pragma interface
#include "CVector3.h"

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
class SClsnContact;

// The name is a return type recorded in the template symbols
// TestCollisionWithSubVolumes__H1Z...; its values are not recovered by name: the
// sphere tests return 1 when apart and 2 on contact.
enum EClsnState {};

// The bit at +16 and the result word at +24 are established by the volume tests;
// their names are descriptive and the other storage is unknown.
class CCollision {
  public:
    void SwapOrder();
    void AddContact(const SClsnContact &);

    unsigned char unknown_00[16];
    unsigned int lineTest : 1;
    unsigned int unknown_10 : 31;
    unsigned char unknown_14[4];
    int result;
};

// The virtual functions are declared in the order of _vt.7IVolume, whose entries
// name each one. The mangled names do not record return types: EClsnState for the
// tests is taken from the template symbols that dispatch them.
class IVolume {
  public:
    virtual ~IVolume() {}
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
};

#endif
