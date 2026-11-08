// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Volume.h"

extern "C" float sqrtf(float);

static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}

static inline void Normalize(CVector3 &v) {
    float length = sqrtf(v.LengthSquared());
    if (length != 0.0f)
        v *= 1.0f / length;
}

static inline float Dot(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}

static inline void Transform(CVector3 &out, const CMatrix &m, const CVector3 &p) {
    CVector3 r;
    r.x = p.x * m.row[0].x + p.y * m.row[1].x + p.z * m.row[2].x + m.row[3].x;
    r.y = p.x * m.row[0].y + p.y * m.row[1].y + p.z * m.row[2].y + m.row[3].y;
    r.z = p.x * m.row[0].z + p.y * m.row[1].z + p.z * m.row[2].z + m.row[3].z;
    out = r;
}

void CHierObject::OnCollision(const CCollision &collision) {
    if (hierFlags.disabled)
        return;
    ISceneNode *other = (ISceneNode *)(collision.first != (void *)this ? collision.first : collision.second);
    if (other->AsBullet())
        return;
    if (hierFlags.root) {
        for (SClsnContact *contact = collision.contacts; contact; contact = contact->next)
            SetHierCollisionState(contact->feature);
        ((CStaticObject *)this)->AttemptCollisionDamage(collision);
    }
    if (hierFlags.collided) {
        if (hierFlags.root && other->AsPlayerObject()) {
            CVector3 contactPosition, otherPosition, push, normal;
            contactPosition = collision.contacts->position;
            other->GetPosition(otherPosition);
            SetSub(push, contactPosition, otherPosition);
            Normalize(push);
            if (pathState > 1) {
                SetSub(normal, transform.GetPos(), pathPosition);
                Normalize(normal);
            } else if (ScriptMoving() || ScriptRotating() || ScriptScaling()) {
                CVector3 local;
                CMatrix inverse;
                if (!CMatrix::s_ClassInit)
                    CMatrix::InitClass();
                inverse.FastInverse(transform);
                Transform(local, inverse, contactPosition);
                orientation.GetMatrix(inverse);
                inverse.SetPos(pathPosition);
                Transform(local, inverse, local);
                SetSub(normal, contactPosition, local);
                Normalize(normal);
            }
            if (!(Dot(push, normal) >= 0.0f)) {
                CollisionResetMovement();
                for (CHierObject *child = firstChild; child; child = child->nextSibling) {
                    child->SetInheritedMatrix(transform);
                    ((ISceneNode *)child)->OnCollision(collision);
                }
            }
            hierFlags.collided = 0;
            return;
        }
        for (SClsnContact *contact = collision.contacts; contact; contact = contact->next)
            touching.Add((ISceneNode *)other->AsAnimObject());
    }
    for (CHierObject *child = firstChild; child; child = child->nextSibling)
        ((ISceneNode *)child)->OnCollision(collision);
}
