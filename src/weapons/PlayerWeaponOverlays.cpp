// AI-assisted reconstruction from GR8E69; see docs/Weapons.md.
#include "CameraShake.h"
void CPlayerWeaponObject::FreeAimOverlay() {
    if (aimOverlay) {
        aimOverlay->~CSprite();
        aimOverlay = 0;
    }
}
void CPlayerWeaponObject::FreeCrosshair() {
    if (crosshair) {
        crosshair->~CSprite();
        crosshair = 0;
    }
}
