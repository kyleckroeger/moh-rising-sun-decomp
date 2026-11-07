// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundEvents.h"
void SoundEntry::Calculate3D() {
    int oldAzimuth = azimuth;
    float oldDistance = distance;
    if (emitter) {
        if (emitter->GetObjRef() == 0 && emitter->GetType() == 0) {
            if (looping) field_4c = 1;
        } else {
            azimuth = emitter->GetAzimuth();
            distance = emitter->GetDistance();
        }
        if (oldDistance != distance) updates |= 2;
        if (oldAzimuth != azimuth) updates |= 4;
    }
}
