// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "profile.h"
#include <map>
#include "CameraShake.h"
#include "SceneObjects.h"
typedef _STL::map<int, CPlayerWeaponObject *> PlayerWeaponMap;
// CPlayerObject's view keeps the map as raw storage so that other users need no STL headers.
#define WEAPONS ((const PlayerWeaponMap *)weaponMap)
#define MUTABLE_WEAPONS ((PlayerWeaponMap *)weaponMap)
#include "MathFun.h"
#include "ScriptTimers.h"
#include "Weapon.h"
void CPlayerObject::SetScript(BSObject *object) {
    script = object;
    for (PlayerWeaponMap::iterator it = MUTABLE_WEAPONS->begin(); it != MUTABLE_WEAPONS->end(); ++it) {
        CPlayerWeaponObject *weapon = it->second;
        if (weapon) ((CAnimObjectInterface *)weapon)->SetScript(object);
    }
}
void CPlayerObject::TriggerScriptEvent(int event, void *data, bool flag) {
    if (script) BSObjectTriggerEvent(script, event, data, 0, flag);
}
void CPlayerObject::SetClimbingState(bool climbing) {
    pathBits.climbing = climbing;
    if (pathBits.climbing) pathBits.climbCleared = 0;
}
void CPlayerObject::SetFallingDamageState(bool enabled) { flags.fallingDamage = enabled; }
void CPlayerObject::FireMyWeapon(float value) {
    if (GetCurrentPlayerWeapon()) {
        CPlayerWeaponObject *weapon = GetCurrentPlayerWeapon();
        float yawKick = weapon->recoilYaw;
        float pitchKick = weapon->recoilPitch;
        recoilPitch += pitchKick;
        recoilYaw += yawKick;
    }
    GetCurrentWeapon()->Shoot((EWeaponShootType)0, value, -1.0f);
}
void CPlayerObject::SetZoom(float zoom) {
    float low = 5.0f;
    if (pathBits.meleeImmune && (mountedGun->flags & 0x80000))
        low = MathFunClamp(35.0f / mountedGun->zoomFov, low, 35.0f);
    if (!(__builtin_fabsf(zoom) <= 0.0f)) {
        targetFov = zoom;
        targetFov = MathFunClamp(zoom, low, 35.0f);
    }
    TransitionFOV(targetFov, 2100.0f);
}
int CPlayerObject::GetNumGrenades() const {
    if (!thrownWeapon) return 0;
    int count = thrownWeapon->reserveAmmo + thrownWeapon->loadedAmmo;
    return (unsigned short)count;
}
void CPlayerObject::ThrowGrenade(float value) { thrownWeapon->Shoot((EWeaponShootType)2, value, -1.0f); }
