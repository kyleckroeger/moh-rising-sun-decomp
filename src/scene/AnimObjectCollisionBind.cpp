// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "AnimatedVolume.h"

void CAnimObject::BindLocationCollision(EALA::Character::CollisionData *data) {
    if (animatedVolume)
        animatedVolume->Set(data);
}
