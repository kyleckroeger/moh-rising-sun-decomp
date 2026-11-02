// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"

namespace EALA {
namespace Character {
class Skeleton {
  public:
    const CMatrix *FindAttachPoint(const char *);
};
}
}

int CSoldierObject::GetAttachPointMatrix(char *name, CMatrix *out) {
    const CMatrix *point = ((EALA::Character::Skeleton *)((CAnimObject *)this)->skeleton)->FindAttachPoint(name);
    if (!point)
        return 0;
    *out = *point;
    return 1;
}
