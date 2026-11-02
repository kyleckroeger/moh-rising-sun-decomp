// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original world_object.cpp; member names are descriptive.
#include "SceneNode.h"
#include "WorldObject.h"
#include "IVolume.h"

IVolume *CWorldObject::GetLocalBoundingVolume(ISceneNode::EVolumeType) const { return (IVolume *)worldVolumeStorage; }
IVolume *CWorldObject::GetWorldBoundingVolume(ISceneNode::EVolumeType) const { return (IVolume *)worldVolumeStorage; }
int CWorldObject::IsVisible(CDrawContext &) const { return 1; }
unsigned int CWorldObject::IsDrawEnabled() const { return drawEnabled; }
EClsnId CWorldObject::GetCollisionId() const { return (EClsnId)0; }
