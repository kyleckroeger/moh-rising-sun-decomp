// AI-assisted scoped reconstruction from GR8E69; see docs/LightVolumes.md and
// docs/Collision.md.
#ifndef GAME_VOLUME_H
#define GAME_VOLUME_H
#pragma interface
#include "CVector3.h"

inline void *operator new(unsigned int, void *p) { return p; }

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

// 44 bytes as stored by AddContact, plus the link it fills at +44. The field
// names are descriptive; the separation is adjusted by CCollision's word at +20.
struct SClsnContact {
    CVector3 normal;
    CVector3 position;
    float separation;
    int feature;
    int subVolume;
    SClsnContact *next;
};

// Contact pool at CCollision::ms_contactPool; the words at +4 (free list), +8
// (capacity) and +12 (used count) are established by AddContact and ClearContacts.
struct SClsnContactPool {
    int unknown_00;
    SClsnContact *freeList;
    int capacity;
    int used;
    int unknown_10;

    SClsnContact *Allocate() {
        if (used < capacity) {
            SClsnContact *contact = freeList;
            SClsnContact *next = *(SClsnContact **)contact;
            if (contact)
                new (contact) SClsnContact;
            used++;
            freeList = next;
            return contact;
        }
        return 0;
    }
    void Free(SClsnContact *contact) {
        used--;
        *(SClsnContact **)contact = freeList;
        freeList = contact;
    }
};

// 36 bytes (CCSGVolume strides by 36). Field names are descriptive; the two words
// at +0 are what SwapOrder exchanges, and the contacts form a list through +44.
class CCollision {
  public:
    CCollision();
    CCollision(const CCollision &);
    ~CCollision();
    CCollision &operator=(const CCollision &);
    CCollision &operator+=(const CCollision &);
    EClsnState Test();
    void SwapOrder();
    void AddContact(const SClsnContact &);
    void ClearContacts();
    // Descriptive inline; the volume tests raise the result in this shape.
    void RaiseResult(int value) {
        if (value > result)
            result = value;
    }

    static SClsnContactPool ms_contactPool;

    void *first;
    void *second;
    float unknown_08;
    float depth;
    unsigned int lineTest : 1;
    unsigned int unknown_10a : 1;
    unsigned int unknown_10b : 2;
    unsigned int unknown_10c : 1;
    unsigned int unknown_10d : 1;
    unsigned int unknown_10e : 26;
    float separationOffset;
    int result;
    SClsnContact *contacts;
    int contactCount;
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
