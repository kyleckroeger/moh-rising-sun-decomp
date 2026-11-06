// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ObserverTypes.h"
#include "GameSettings.h"

BPDLightVolume *CHierObject::GetLightVolume() {
    if (hierFlags.root)
        return lightVolumes.GetVolume();
    return ((IMovingSceneNode *)root)->GetLightVolume();
}

