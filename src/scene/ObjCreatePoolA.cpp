// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp; the pools are the file's local statics.
#include "PlacementNew.h"
#include "ScriptPool.h"
#include "Box.h"
#include "Sphere.h"
#include "AnimatedVolume.h"

extern BSUtilObjectInstanceMemoryAllocator g_pVolumeAllocator;
extern BSUtilObjectInstanceMemoryAllocator g_pAnimatedVolumeAllocator;

void *AllocateAnimatedVolume() { return g_pAnimatedVolumeAllocator.GetFreeElement(1); }
void FreeAnimatedVolume(void *volume) { g_pAnimatedVolumeAllocator.FreeElement(volume); }

