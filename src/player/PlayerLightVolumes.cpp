// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "profile.h"
#include <map>
#include "CameraShake.h"
#include "SceneObjects.h"
typedef _STL::map<int, CPlayerWeaponObject *> PlayerWeaponMap;
// CPlayerObject's view keeps the map as raw storage so that other users need no STL headers.
#define WEAPONS ((const PlayerWeaponMap *)weaponMap)
#define MUTABLE_WEAPONS ((PlayerWeaponMap *)weaponMap)
void CPlayerObject::SetDetonationTimeForPlayerTurret(float time) { turretWeapon->shootValue2 = time; }
void CPlayerObject::EnterLightVolume(BPDLightVolume *volume) {
    for (PlayerWeaponMap::iterator it = MUTABLE_WEAPONS->begin(); it != MUTABLE_WEAPONS->end(); ++it)
        ((IMovingSceneNode *)it->second)->EnterLightVolume(volume);
    if (body) ((IMovingSceneNode *)body)->EnterLightVolume(volume);
}
void CPlayerObject::ExitLightVolume(BPDLightVolume *volume) {
    for (PlayerWeaponMap::iterator it = MUTABLE_WEAPONS->begin(); it != MUTABLE_WEAPONS->end(); ++it)
        ((IMovingSceneNode *)it->second)->ExitLightVolume(volume);
    if (body) ((IMovingSceneNode *)body)->ExitLightVolume(volume);
}
void CPlayerObject::SetLightVolumeTransitionDuration(float duration) {
    for (PlayerWeaponMap::iterator it = MUTABLE_WEAPONS->begin(); it != MUTABLE_WEAPONS->end(); ++it)
        ((IMovingSceneNode *)it->second)->SetLightVolumeTransitionDuration(duration);
    if (body) ((IMovingSceneNode *)body)->SetLightVolumeTransitionDuration(duration);
}
