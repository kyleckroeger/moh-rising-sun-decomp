// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original world_object.cpp.
#include "WorldObject.h"

CWorldObject *CWorldObject::AsWorldObject() { return this; }
const CWorldObject *CWorldObject::AsWorldObject() const { return this; }
