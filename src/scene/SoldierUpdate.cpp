// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "Box.h"

void CSoldierObject::AttemptUpdate(float dt) {
    collidedThisFrame = 0;
    ((CAnimObject *)this)->AttemptUpdate(dt);
    volumeLists[0].Clear();
    volumeLists[1].Clear();
    volumeLists[2].Clear();
    moveBlend = 0.0f;
    ((ISceneNode *)this)->UpdateCollisionVolumes();
}

void CSoldierObject::CommitUpdate() {
    CObjectFadeManager *fade = (CObjectFadeManager *)fadeStorage;
    CAnimObject *self = (CAnimObject *)this;
    self->CommitUpdate();
    self->previousPosition = self->transform.GetPos();
    if (fade->AreFading() && !weaponsRemoved && fade->HalfDone()) {
        weaponsRemoved = 1;
        RemoveWeapons();
    }
}

void CSoldierObject::UpdateLocalBoundingVolume(ISceneNode::EVolumeType type, const CVector3 &min, const CVector3 &max) {
    if (type == 3)
        ((CVolBox *)((CAnimObject *)this)->localVolumeType3)->SetCorners(min, max);
}

IVolume *CSoldierObject::GetLocalBoundingVolume(ISceneNode::EVolumeType type) const {
    const CAnimObject *self = (const CAnimObject *)this;
    switch (type) {
    case 3:
        return self->localVolumeType3;
    case 0:
        return self->localVolumeType0;
    case 1:
        return (IVolume *)localVolumeType1;
    case 2:
        return (IVolume *)localVolumeType2;
    case 4:
        return 0;
    }
    return 0;
}

IVolume *CSoldierObject::GetWorldBoundingVolume(ISceneNode::EVolumeType type) const {
    const CAnimObject *self = (const CAnimObject *)this;
    switch (type) {
    case 3:
        return self->worldVolumeType3;
    case 0:
        return self->worldVolumeType0;
    case 1:
        return (IVolume *)worldVolumeType1;
    case 2:
        return (IVolume *)worldVolumeType2;
    case 4:
        return 0;
    }
    return 0;
}
