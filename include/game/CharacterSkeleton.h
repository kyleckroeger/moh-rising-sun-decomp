#ifndef GAME_CHARACTER_SKELETON_H
#define GAME_CHARACTER_SKELETON_H
// AI-assisted scoped view of EALA::Character::Skeleton; see docs/Animation.md. The first word is the
// resource pointer the symbol Bind__...Pointer1Z...SkeletonData takes. Member names are inferred.
#include "EAGLTransform.h"
namespace EALA {
namespace Character {
class SkeletonData {
  public:
    int FindJoint(const char *) const;
    int FindJoint(unsigned int) const;
    bool IsJointBvExcluded(int);
    void PrepForSkinning(EAGL::Transform *);
};
class Skeleton {
  public:
    SkeletonData *data;   // +0
    unsigned char unknown_4[48];
    EAGL::Transform *matrices; // +52: 64-byte transforms, per FindAttachPoint.
    bool HasData() const { return data != 0; }
    int FindJoint(const char *) const;
    int FindJoint(unsigned int) const;
    bool IsJointBvExcluded(int) const;
    void PrepForSkinning();
};
}
}
#endif
