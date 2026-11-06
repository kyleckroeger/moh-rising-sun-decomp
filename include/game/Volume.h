// AI-assisted scoped reconstruction from GR8E69; see docs/LightVolumes.md and
// docs/Collision.md.
#ifndef GAME_VOLUME_H
#define GAME_VOLUME_H
#pragma interface
#include "CVector3.h"
#include "PlacementNew.h"
#include "IVolume.h"

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

#endif
