// CCSGVolume, all of csg_volume.cpp: a CSG volume tests each sub-volume of its hierarchy
// object into one of twenty collision records and keeps the nearest contact. CheckContacts
// adds a contact for every record at that depth. The constructor and destructor keep the
// virtual table with this file.
#include "CSGVolume.h"
#include "AnimatedVolume.h"
#include "Box.h"
#include "CDBObject.h"
#include "Capsule.h"
#include "Fiber.h"
#include "Frustum.h"
#include "WorldVolume.h"
#include "SceneObjects.h"

CCSGVolume::CCSGVolume() : object(0), index(0) {}
CCSGVolume::~CCSGVolume() {}
void CCSGVolume::Init(int i, CHierObject *o) {
    index = i;
    object = o;
}

EClsnState CCSGVolume::TestCollision(const CLine3 &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::CheckContacts(int *indices, float *depths, CCollision *collisions, int count, CCollision &collision,
                                     bool add) const {
    float nearest = 3.4028235e38f;
    int best = -1;
    for (int i = 0; i < count; i++) {
        if (!(depths[i] >= nearest)) {
            nearest = depths[i];
            best = indices[i];
        }
    }
    int result;
    if (nearest == 3.4028235e38f) {
        if (add)
            collision.RaiseResult(1);
        result = 1;
    } else {
        CCollision &nearestCollision = collisions[best];
        collision.depth = nearestCollision.depth;
        collision.lineTest = nearestCollision.lineTest;
        result = nearestCollision.result;
        if (add) {
            for (int i = 0; i < count; i++) {
                if (depths[i] == nearest) {
                    int subVolume = indices[i];
                    nearestCollision = collisions[subVolume];
                    collision.RaiseResult(result);
                    if (result == 2) {
                        SClsnContact contact;
                        const SClsnContact *source = nearestCollision.contacts;
                        contact.separation = source->separation;
                        contact.normal = source->normal;
                        contact.position = source->position;
                        contact.feature = source->feature;
                        contact.subVolume = subVolume;
                        collision.AddContact(contact);
                    }
                }
            }
        }
    }
    return (EClsnState)result;
}

EClsnState CCSGVolume::TestCollision(const IVolume &other, CCollision &collision, bool flag) const {
    collision.SwapOrder();
    return other.TestCollision(*this, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CVolBox &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CVolSphere &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CVolCapsule &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CCDBObject &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CVector3 &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CPlane &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CTriangle &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CVolFiber &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CWorldVolume &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CAnimatedVolume &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

EClsnState CCSGVolume::TestCollision(const CCSGVolume &other, CCollision &collision, bool flag) const {
    int indices[20];
    float depths[20];
    CCollision collisions[20];
    int count = 0;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume && volume->TestCollision(other, collisions[i], true) == 2) {
            indices[count] = i;
            depths[count] = collisions[i].depth;
            count++;
        }
    }
    return CheckContacts(indices, depths, collisions, count, collision, flag);
}

IVolume *CCSGVolume::Create() const { return new CCSGVolume; }

void CCSGVolume::GetExtents(CVector3 &low, CVector3 &high) const {
    CVector3 subLow;
    CVector3 subHigh;
    low.x = 3.4028235e38f;
    low.y = 3.4028235e38f;
    low.z = 3.4028235e38f;
    high.x = -3.4028235e38f;
    high.y = -3.4028235e38f;
    high.z = -3.4028235e38f;
    for (int i = 0; i < index; i++) {
        IVolume *volume = object->GetSubVolume(i);
        if (volume) {
            volume->GetExtents(subLow, subHigh);
            // A named reference reproduces the saved address register the original keeps.
            CVector3 &lowRef = subLow;
            if (!(subLow.x >= low.x))
                low.x = subLow.x;
            if (!(subLow.y >= low.y))
                low.y = subLow.y;
            if (!(lowRef.z >= low.z))
                low.z = lowRef.z;
            if (!(subHigh.x <= high.x))
                high.x = subHigh.x;
            if (!(subHigh.y <= high.y))
                high.y = subHigh.y;
            if (!(subHigh.z <= high.z))
                high.z = subHigh.z;
        }
    }
}

void CCSGVolume::TransformedCopy(const IVolume &, const CMatrix &) {}
