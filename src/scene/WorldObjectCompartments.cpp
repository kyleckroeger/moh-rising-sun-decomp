// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original world_object.cpp; member names are descriptive.
#include "SceneNode.h"
#include "WorldObject.h"
#include "Compartment.h"

CCompartment *CWorldObject::GetCompartment(int index) {
    for (CompartmentIterator it = BeginCompartments(); it != EndCompartments(); ++it) {
        if ((*it)->index == index)
            return *it;
    }
    return 0;
}

void CWorldObject::HandleExplosion(const CExplosion &explosion) {
    for (CompartmentIterator it = BeginCompartments(); it != EndCompartments(); ++it)
        (*it)->HandleExplosion(explosion);
}
