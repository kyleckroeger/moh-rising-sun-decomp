// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundEvents.h"
#include "PlacementNew.h"

void PositionSoundEmitter::Clone(SoundEmitter *out) const {
    if (out)
        new (out) PositionSoundEmitter(*this);
}

void PositionSoundEmitter::SetPosition(const CVector3 &v) {
    x = v.x;
    y = v.y;
    z = v.z;
}
