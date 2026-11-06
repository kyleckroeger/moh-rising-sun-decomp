// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"

// Method-only views. The animated object carries a second table pointer at +0x38, and the
// skeleton accessor is called through its slot at +0x10; the holder's other slot is a
// placeholder for that position only. The symbol establishes FindAttachPoint's nesting.
namespace EALA {
namespace Character {
class Skeleton {
  public:
    const CMatrix *FindAttachPoint(const char *);
};
}
}
class CSkeletonHolder {
  public:
    virtual void Slot8();
    virtual EALA::Character::Skeleton *GetSkeleton();
};

void CHierAnimObject::AttemptUpdate(float dt) {
    AttemptUpdateFromMotion(dt);
    ((IMovingSceneNode *)animObject)->SetTMLocalToWorld(transform);
    ((ISceneNode *)animObject)->AttemptUpdate(dt);
    CMatrix attach;
    attach = *((CSkeletonHolder *)((char *)animObject + 0x38))->GetSkeleton()->FindAttachPoint("JO_gun");
    for (CHierObject *child = firstChild; child; child = child->nextSibling) {
        child->SetAttachPointMatrix(attach);
        child->SetInheritedMatrix(transform);
        ((ISceneNode *)child)->AttemptUpdate(dt);
    }
    AttemptUpdateOfAttachedObjects();
}

void CHierAnimObject::CommitUpdate() {
    ((ISceneNode *)animObject)->CommitUpdate();
    CHierObject::CommitUpdate();
}

void CHierAnimObject::OnCollision(const CCollision &collision) {
    ((ISceneNode *)animObject)->OnCollision(collision);
    CHierObject::OnCollision(collision);
}

CVector3 CHierAnimObject::GetWorldLinearVelocity() const {
    CVector3 own = CHierObject::GetWorldLinearVelocity();
    CVector3 object = ((ISceneNode *)animObject)->GetWorldLinearVelocity();
    own += object;
    return own;
}
