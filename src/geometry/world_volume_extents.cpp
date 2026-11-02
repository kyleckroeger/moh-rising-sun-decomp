// Fragment of world_volume.cpp: the extents over all sub-volumes.
#include "WorldVolume.h"
#include "CDBObject.h"

// The plain references below add no storage; they reproduce the code shape.
static inline float MinOf(float a, float b) {
    if (!(a <= b))
        a = b;
    return a;
}
static inline float MaxOf(float a, float b) {
    if (!(a >= b))
        a = b;
    return a;
}

void CWorldVolume::UpdateExtents() {
    dwi::list<const CCDBObject *> &list = subVolumes;
    if (list.count != 0) {
        CVector3 &initLow = extentLow;
        CVector3 &initHigh = extentHigh;
        initLow.x = 3.4028235e38f;
        initLow.z = 3.4028235e38f;
        initLow.y = 3.4028235e38f;
        initHigh.x = -initLow.x;
        initHigh.y = -initLow.y;
        initHigh.z = -initLow.z;
        dwi::list_iter<const CCDBObject *> i;
        for (i = list.begin(); i != subVolumes.end(); ++i) {
            const CCDBObject *object = *i;
            CVector3 low;
            CVector3 high;
            object->GetExtents(low, high);
            CVector3 &lo = extentLow;
            CVector3 &hi = extentHigh;
            lo.x = MinOf(low.x, lo.x);
            lo.y = MinOf(low.y, lo.y);
            lo.z = MinOf(low.z, lo.z);
            hi.x = MaxOf(high.x, hi.x);
            hi.y = MaxOf(high.y, hi.y);
            hi.z = MaxOf(high.z, hi.z);
        }
    } else {
        extentLow.Zero();
        extentHigh.Zero();
    }
}
