// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Tank_object.cpp.
#include "SceneObjects.h"
#include "AIDoodad.h"

CAIDoodad *CTankObject::GetAIDoodad() { return (CAIDoodad *)aiDoodad; }
const CAIDoodad *CTankObject::GetAIDoodad() const { return (const CAIDoodad *)aiDoodad; }
CTankObject *CTankObject::AsHierTankObject() { return this; }
