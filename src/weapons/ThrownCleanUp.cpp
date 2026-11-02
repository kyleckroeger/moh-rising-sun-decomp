// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Thrown.h"
void FreeSphereVolume(void *);
void CThrownObject::CleanUp() {
    ISceneNode *node = (ISceneNode *)this;
    void *local = (void *)node->GetLocalBoundingVolume((ISceneNode::EVolumeType)3);
    if (local) FreeSphereVolume(local);
    void *world = (void *)node->GetWorldBoundingVolume((ISceneNode::EVolumeType)3);
    if (world && world != local) FreeSphereVolume(world);
    worldVolume = 0;
    field_13c = 0;
}
