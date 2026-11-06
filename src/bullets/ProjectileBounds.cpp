// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
int CProjectileBullet::GetLocalBoundingVolume(ISceneNode::EVolumeType) const { return (int)localVolume; }
int CProjectileBullet::GetWorldBoundingVolume(ISceneNode::EVolumeType) const { return (int)worldVolume; }
