// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "GameSettings.h"
int CPlayerObject::IsInvincible() const {
    return flags.invincible || g_cheats.invincible;
}
