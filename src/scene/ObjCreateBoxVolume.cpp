// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp; the pool is one of the file's statics.
#include "PlacementNew.h"
#include "ScriptPool.h"
#include "Box.h"
#include "ObjCreate.h"

void *AllocateBoxVolume() {
    void *memory = g_pVolumeAllocator.GetFreeElement(1);
    if (memory)
        new (memory) CVolBox;
    return memory;
}
