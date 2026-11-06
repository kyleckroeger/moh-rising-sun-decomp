// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"

// Method-only views; the symbol establishes the nesting.
namespace EALA {
namespace Character {
class Skeleton {
  public:
    class Partition;
};
class Choreographer {
  public:
    void SetState(const char *, EALA::Task *, float, const Skeleton::Partition *);
};
}
}

extern void FreeSphereVolume(void *);

void CAnimObject::CleanUp() {
    if (localVolumeType0) {
        FreeSphereVolume(localVolumeType0);
        localVolumeType0 = 0;
    }
    if (worldVolumeType0) {
        FreeSphereVolume(worldVolumeType0);
        worldVolumeType0 = 0;
    }
}

int CAnimObject::SetAnimationState(const char *name, int value) {
    if (!name)
        return 1;
    stateValue = value;
    stateOwner = this;
    ((EALA::Character::Choreographer *)choreographer)->SetState(name, (EALA::Task *)&stateTask, 0.25f, 0);
    stateName = name;
    return 0;
}

int CAnimObject::SetAnimationState(const char *name, EALA::Task *task) {
    if (!name)
        return 1;
    ((EALA::Character::Choreographer *)choreographer)->SetState(name, task, 0.25f, 0);
    return 0;
}
