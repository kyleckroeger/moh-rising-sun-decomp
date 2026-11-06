// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "CSGVolume.h"
#include "CameraShake.h"

void CHierObject::AttemptUpdateOfAttachedObjects() {
    ((CAttachableObject *)this)->UpdateAttachedObjects();
    if (hierFlags.mountedGun)
        g_scene.GetPlayer(0)->SetPlayersMountedMGPositions(&transform);
    if (hierFlags.root) {
        CVector3 min, max;
        ((CCSGVolume *)csgVolume)->CCSGVolume::GetExtents(min, max);
        sortMinX->value = min.x;
        sortMinY->value = min.y;
        sortMaxX->value = max.x;
        sortMaxY->value = max.y;
    }
}
