// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
void CPlayerObject::SetHideWeapon(bool hide) {
    if (flags.hideWeapon) {
        if (!hide) ((ISceneNode *)this)->TriggerScriptEvent(259, 0, false);
    } else if (hide) {
        ((ISceneNode *)this)->TriggerScriptEvent(258, 0, false);
    }
    flags.hideWeapon = hide;
}
