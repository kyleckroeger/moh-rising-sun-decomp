// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
void CPlayerObject::ResetGrenadeCount(int count) {
    if (thrownWeapon) {
        thrownWeapon->Empty();
        thrownWeapon->AddAmo(count);
        thrownWeapon->DoneReloading();
    }
}
void CPlayerObject::BeginStand() {
    pathFlags = (pathFlags & ~0x400000) | 0x200000;
    field_28c = 0.0400000028f;
}
void CPlayerObject::BeginCrouch() {
    if (flags.canCrouch) {
        pathFlags |= 0x600000;
        field_28c = -0.0400000028f;
    }
}
