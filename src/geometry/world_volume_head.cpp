// Fragment of world_volume.cpp: construction, destruction and adding or removing sub-
// volumes, on the dwi list.
#include "WorldVolume.h"
#include "CDBObject.h"

CWorldVolume::CWorldVolume(int count) {
    unknown_50 = 0;
    subVolumes.reserve(count);
}

CWorldVolume::~CWorldVolume() {}

void CWorldVolume::AddSubVolume(const CCDBObject &object) {
    subVolumes.push_back(&object);
    UpdateExtents();
}

void CWorldVolume::RemoveSubVolume(const CCDBObject &object) {
    for (dwi::list_iter<const CCDBObject *> i = subVolumes.begin(); i != subVolumes.end(); ++i) {
        if (*i == &object) {
            RemoveSubVolume(i);
            return;
        }
    }
}

void CWorldVolume::RemoveSubVolume(dwi::list_iter<const CCDBObject *> position) {
    subVolumes.erase(position);
    UpdateExtents();
}
