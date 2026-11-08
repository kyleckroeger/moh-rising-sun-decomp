// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "Camera.h"
extern CCamera g_camera;
void CPlayerObject::AimAdjust::Enable() {
    active = 1;
    int zero = 0;
    link.Unlink();
    field_14 = zero;
    field_40 = zero;
    UpdateTarget(g_camera);
}
