// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
int CThrownBullet::GetLocalBoundingVolume(ISceneNode::EVolumeType) const { return (int)localVolume; }
int CThrownBullet::GetWorldBoundingVolume(ISceneNode::EVolumeType) const { return (int)worldVolume; }
