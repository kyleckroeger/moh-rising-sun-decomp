// Fragment of animated_volume.cpp: construction, destruction and Set. The destructor is
// the key function, so the table is emitted here; the class allocates through
// AllocateAnimatedVolume and frees through FreeAnimatedVolume. Their inline operator
// copies are not kept by the original link.
#include "AnimatedVolume.h"
#include "Capsule.h"

CAnimatedVolume::CAnimatedVolume() {
    data = 0;
    collisionFlag = 0;
    unknown_30 = 0;
    subVolumes = 0;
    unknown_2c = 0;
}

CAnimatedVolume::~CAnimatedVolume() { delete[] subVolumes; }

void CAnimatedVolume::Set(EALA::Character::CollisionData *collisionData) {
    data = collisionData;
    collisionFlag = 0;
}
