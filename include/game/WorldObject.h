// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_WORLD_OBJECT_H
#define GAME_WORLD_OBJECT_H

#include "SceneNode.h"
// Method-only view of the world (compartment) object held by CScene at +0x18.
class CExplosion;
class IVolume;
class CCompartment;
// A sorted bound entry; only the key at +8 is established.
struct CWorldBound {
    unsigned char unknown_0[8];
    float key;
};
class CWorldObject {
  public:
    // Sixteen-byte compartment request; the four entries are sign-extended shorts.
    struct SAcquireSet {
        int compartments[4];
    };
    // The compartment list at +0x60 embeds its end node. Names are descriptive.
    struct CompartmentNode {
        CompartmentNode *previous;
        CompartmentNode *next; // +0x04
        CCompartment *compartment; // +0x08
    };
    class CompartmentIterator {
      public:
        CompartmentNode *node;
        CompartmentIterator(CompartmentNode *n) : node(n) {}
        CompartmentIterator(const CompartmentIterator &other) : node(other.node) {}
        bool operator!=(const CompartmentIterator &other) const { return node != other.node; }
        void operator++() { node = node->next; }
        CCompartment *operator*() const { return node->compartment; }
    };
    CompartmentIterator BeginCompartments() { return CompartmentIterator(compartments.next); }
    CompartmentIterator EndCompartments() { return CompartmentIterator(&compartments); }
    void Acquire(const SAcquireSet &, bool, bool);
    CCompartment *GetCompartment(int);
    void DrawShadows(CDrawContext &);
    void Draw(CDrawContext &);
    void BeginUpdate(float);
    void ServiceDataStreamer(float);
    void HandleExplosion(const CExplosion &);
    IVolume *GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    IVolume *GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    int IsVisible(CDrawContext &) const;
    unsigned int IsDrawEnabled() const;
    EClsnId GetCollisionId() const;
    CWorldObject *AsWorldObject();
    const CWorldObject *AsWorldObject() const;
    unsigned char unknown_0[0x20];
    CWorldBound *bounds[4]; // +0x20; the sorted bound entries low x, high x, low y, high y.
    unsigned char unknown_30[0x60 - 0x30];
    CompartmentNode compartments; // +0x60
    unsigned char unknown_6c[4];
    int streaming; // +0x70
    unsigned char unknown_74[0xd0 - 0x74];
    unsigned char worldVolumeStorage[0x130 - 0xd0]; // +0xd0; a CWorldVolume, returned for every volume type.
    unsigned int drawEnabled; // +0x130
};

#endif
