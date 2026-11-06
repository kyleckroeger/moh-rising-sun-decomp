// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Thrown.h"
#include "IDestructible.h"
#include "ScriptTimers.h"
class CSurfaceType {
  public:
    unsigned char unknown_0[12];
    short soundGroup;  // +12; the flag argument is whether it is non-negative.
    unsigned short id; // +14
    static int GetSoundIndex(unsigned short, bool);
};
struct ThrownContact {
    CVector3 normal;
    unsigned char unknown_10[0x24 - 0x10];
    CSurfaceType *surface; // +0x24
    unsigned char unknown_28[4];
    ThrownContact *next;   // +0x2c
};
static inline void SetNegated(CVector3 &r, const CVector3 &v) { r.x = -v.x; r.y = -v.y; r.z = -v.z; }
static inline float Dot(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}
void CThrownObject::OnCollision(const CCollision &collision) {
    bool first = collision.first == (void *)this;
    ISceneNode *other = (ISceneNode *)(collision.first != (void *)this ? collision.first : collision.second);
    if (state == -1) {
        if (!other->AsBullet()) {
            for (ThrownContact *contact = (ThrownContact *)collision.contacts; contact; contact = contact->next) {
                CVector3 normal;
                if (first) {
                    SetNegated(normal, contact->normal);
                } else {
                    normal = contact->normal;
                }
                if (!(Dot(velocity, normal) >= 0.0f)) {
                    field_320 += normal;
                    bits.touched2 = 1;
                }
                if (!(contact->normal.z <= 0.7f)) {
                    bits.touched = 1;
                    if (pendingSound == 0 && other->AsWorldObject()) {
                        if (contact->surface) pendingSound = CSurfaceType::GetSoundIndex(contact->surface->id, contact->surface->soundGroup != 0);
                        else pendingSound = 40;
                    }
                }
            }
        }
    } else if (state != 0) {
        if (state == 1 && (other->AsAnimObject() || other->AsPlayerObject())) {
            BSObject *self = (BSObject *)((ISceneNode *)this)->GetScriptObject();
            BSObjectTriggerEvent(self, 224, (void *)((ISceneNode *)other)->GetScriptObject(), 0, false);
            g_scene.ExcludeCollision(*(ISceneNode *)this, *other, -1);
        } else {
            for (ThrownContact *contact = (ThrownContact *)collision.contacts; contact; contact = contact->next) {
                if (!(contact->normal.z <= 0.7f)) {
                    bits.touched = 1;
                    if (state != 2) {
                        BSObjectTriggerEvent((BSObject *)((ISceneNode *)this)->GetScriptObject(), 225, 0, 0, false);
                        state = 2;
                    }
                    CVector3 zero(0.0f, 0.0f, 0.0f);
                    velocity = zero;
                    if (pendingSound == 0 && other->AsWorldObject()) {
                        if (contact->surface) pendingSound = CSurfaceType::GetSoundIndex(contact->surface->id, contact->surface->soundGroup >= 0);
                        else pendingSound = 40;
                    }
                }
            }
        }
    }
}
void CThrownObject::UpdateCollisionVolumes() {
    if (field_13c) {
        ((IVolume *)field_13c)->TransformedCopy(*worldVolume, transform);
        CVector3 low, high;
        ((IVolume *)field_13c)->GetExtents(low, high);
        sortMinX->value = low.x;
        sortMinY->value = low.y;
        sortMaxX->value = high.x;
        sortMaxY->value = high.y;
    } else {
        CVector3 position;
        ((ISceneNode *)this)->GetPosition(position);
        sortMinX->value = position.x;
        sortMinY->value = position.y;
        sortMaxX->value = position.x;
        sortMaxY->value = position.y;
    }
}
