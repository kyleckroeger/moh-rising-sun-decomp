// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original Soldier_object.cpp; member names are descriptive.
#include "SceneObjects.h"

CAIDoodad *CSoldierObject::GetAIDoodad() { return (CAIDoodad *)aiDoodad; }
const CAIDoodad *CSoldierObject::GetAIDoodad() const { return (const CAIDoodad *)aiDoodad; }
CSoldierObject *CSoldierObject::AsSoldierObject() { return this; }
const CSoldierObject *CSoldierObject::AsSoldierObject() const { return this; }
