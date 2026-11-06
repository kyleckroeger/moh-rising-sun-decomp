// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
void CPlayerObject::PeekUp(float height) { field_28c = (height * 1.1f - field_270) * 0.4170837998f; }
void CPlayerObject::DuckDown(float height) {
    if (flags.canCrouch) field_28c = ((1.0f - height) * 1.1f - field_270) * 0.4170837998f;
}
