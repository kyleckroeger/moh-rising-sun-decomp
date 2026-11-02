// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
int CPlayerObject::GetLocalBoundingVolume(ISceneNode::EVolumeType type) const {
    switch (type) {
    case 3:
        return (int)localBox;
    case 0:
    case 1:
    case 2:
        return (int)localVolume;
    case 4:
        return 0;
    default:
        return 0;
    }
}
int CPlayerObject::GetWorldBoundingVolume(ISceneNode::EVolumeType type) const {
    switch (type) {
    case 3:
        return (int)worldBox;
    case 0:
    case 1:
    case 2:
        return (int)worldVolume;
    case 4:
        return 0;
    default:
        return 0;
    }
}
