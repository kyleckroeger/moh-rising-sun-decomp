// Fragment of compartment.cpp: the world bounding volume is the local one.
#include "Compartment.h"

int CCompartment::GetWorldBoundingVolume(ISceneNode::EVolumeType type) const {
    return GetLocalBoundingVolume(type);
}
