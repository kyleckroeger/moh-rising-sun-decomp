// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original world_object.cpp; member names are descriptive.
#include "SceneNode.h"
#include "WorldObject.h"
#include "Compartment.h"

// Method-only view of the volume stored at +0xd0.
class CWorldVolume {
  public:
    void GetExtents(CVector3 &, CVector3 &) const;
};

void CWorldObject::Draw(CDrawContext &context) {
    CCompartment::GetShared()->Draw(context);
    for (CompartmentIterator it = BeginCompartments(); it != EndCompartments(); ++it) {
        CCompartment *compartment = *it;
        if (compartment->IsDrawEnabled() && compartment->IsVisible(context))
            compartment->Draw(context);
    }
}

void CWorldObject::DrawShadows(CDrawContext &context) {
    for (CompartmentIterator it = BeginCompartments(); it != EndCompartments(); ++it) {
        CCompartment *compartment = *it;
        if (compartment->IsDrawEnabled() && compartment->IsVisible(context))
            compartment->DrawShadows(context);
    }
}
