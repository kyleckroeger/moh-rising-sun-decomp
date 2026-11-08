// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "AITargeting.h"
#include "AIBehavior.h"
#include "CDBObject.h"
#include "ScriptTimers.h"
#include "Volume.h"

extern const float COLLISION_TOLERANCE;

// Private to the original file, which is not reconstructed yet.
int BumpType(EClsnId, EClsnId);

class CClsnConstraintsBase {
  public:
    void AddConstraints(const CCollision &, bool);
};

struct SurfaceRef {
    unsigned char unknown_0[12];
    short material;
    unsigned short surface;
};

static inline float Dot(const CVector3 &a, const CVector3 &b) {
    float sum = a.x * b.x + a.y * b.y;
    sum += a.z * b.z;
    return sum;
}

void CSoldierObject::OnCollision(const CCollision &collision) {
    ((CAnimObject *)this)->CAnimObject::OnCollision(collision);
    int bumped = 0;
    bool first = collision.first == this;
    ISceneNode *other = (ISceneNode *)(collision.first != this ? collision.first : collision.second);
    if (canBePushed || (!other->AsPlayerObject() && !other->AsSoldierObject()))
        ((CClsnConstraintsBase *)&clsnConstraints)->AddConstraints(collision, first);
    int climbing = 0;
    for (SClsnContact *contact = collision.contacts; contact; contact = contact->next) {
        if (contact->separation > COLLISION_TOLERANCE)
            continue;
        const float walkable = 0.642f;
        const SurfaceRef *surface = (const SurfaceRef *)contact->feature;
        if (!groundCheckState || !(fallSpeed >= 0.0f)) {
            if (contact->normal.z > walkable ||
                (surface && CSurfaceType::IsClimbable(surface->surface, surface->material >= 0) && !(contact->normal.z <= 0.01f)))
                climbing = 1;
        }
        if (!climbing && !(contact->normal.z > walkable))
            bumped = 1;
        if (other->AsWorldObject() && !(contact->normal.z <= moveBlend)) {
            moveBlend = contact->normal.z;
            const SurfaceRef *s = (const SurfaceRef *)contact->feature;
            if (s)
                groundSound = CSurfaceType::GetSoundIndex(s->surface, s->material >= 0);
            else
                groundSound = 40;
        }
    }
    groundCheckPending = climbing;
    if (other->AsSoldierObject() || other->AsPlayerObject()) {
        EClsnId id = ((ISceneNode *)this)->GetCollisionId();
        EClsnId otherId = other->GetCollisionId();
        if (id > 30)
            id = (EClsnId)2;
        if (otherId > 30)
            otherId = (EClsnId)2;
        int event = BumpType(id, otherId);
        if (!other->AsPlayerObject())
            BSObjectTriggerEvent(scriptObject, (unsigned short)event, (void *)other->GetScriptObject(), 0, true);
        CPlayerObject *player = other->AsPlayerObject();
        if (player) {
            if (((ISceneNode *)player)->GetAIObject()->status.GetVarValue((aistatus_var)4) == 1)
                BSObjectTriggerEvent(scriptObject, (unsigned short)event, (void *)other->GetScriptObject(), 0, true);
            if (aiObject)
                ((CBuddyFollowBehavior *)((CAIObject *)aiObject)->FindMatchingBehavior((EBehaviorType)2))->BumpPlayer();
        }
    } else if (other->AsWorldObject()) {
        if (bumped) {
            groundCheckState = 0;
            BSObjectTriggerEvent(scriptObject, 171, (void *)&collision, 0, true);
        } else if (groundCheckPending && groundCheckState) {
            groundCheckState = 0;
            BSObjectTriggerEvent(scriptObject, 141, (void *)&collision, 0, true);
        }
    } else if (other->AsStaticObject() || other->AsHierObject()) {
        CVector3 up(0.0f, 0.0f, 1.0f);
        bool on = false;
        // As in the original, this walks the contact records as if contiguous; it does not follow next.
        const SClsnContact *contact = collision.contacts;
        while (contact) {
            if (!(Dot(contact->normal, up) <= 0.8f)) {
                on = true;
                break;
            }
            contact++;
        }
        if (on) {
            groundCheckState = 0;
            BSObjectTriggerEvent(scriptObject, 141, (void *)&collision, 0, true);
        } else {
            groundCheckState = 0;
            BSObjectTriggerEvent(scriptObject, 192, (void *)&collision, 0, true);
        }
    }
}
