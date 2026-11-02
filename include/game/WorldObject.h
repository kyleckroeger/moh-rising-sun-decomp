// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_WORLD_OBJECT_H
#define GAME_WORLD_OBJECT_H

#include "SceneNode.h"
// Method-only view of the world (compartment) object held by CScene at +0x18.
class CExplosion;
class IVolume;
class CWorldObject {
  public:
    // Sixteen-byte compartment request; the four entries are sign-extended shorts.
    struct SAcquireSet {
        int compartments[4];
    };
    void Acquire(const SAcquireSet &, bool, bool);
    void HandleExplosion(const CExplosion &);
    IVolume *GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    IVolume *GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    int IsVisible(CDrawContext &) const;
    unsigned int IsDrawEnabled() const;
    EClsnId GetCollisionId() const;
    CWorldObject *AsWorldObject();
    const CWorldObject *AsWorldObject() const;
    unsigned char unknown_0[0xd0];
    unsigned char worldVolumeStorage[0x130 - 0xd0]; // +0xd0; a CWorldVolume, returned for every volume type.
    unsigned int drawEnabled; // +0x130
};

#endif
