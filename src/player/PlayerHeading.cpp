// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
float CPlayerObject::GetHeading() {
    if (flags.headingOverride) return mountedHeading;
    return yaw;
}
