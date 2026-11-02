// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "profile.h"
#include <map>
#include "CameraShake.h"
#include "SceneObjects.h"
typedef _STL::map<int, CPlayerWeaponObject *> PlayerWeaponMap;
// CPlayerObject's view keeps the map as raw storage so that other users need no STL headers.
#define WEAPONS ((const PlayerWeaponMap *)weaponMap)
#define MUTABLE_WEAPONS ((PlayerWeaponMap *)weaponMap)
CPlayerWeaponObject *CPlayerObject::GetCurrentPlayerWeapon() const {
    if (pathBits.meleeImmune) return 0;
    return currentWeapon;
}
CWeapon *CPlayerObject::GetCurrentWeapon() const {
    if (pathBits.meleeImmune) return mountedWeapon;
    else if (currentWeapon) return currentWeapon->weapon;
    else return 0;
}
CPlayerWeaponObject *CPlayerObject::GetWeaponByCRC(int crc) const {
    PlayerWeaponMap::const_iterator it = WEAPONS->find(crc);
    if (it != WEAPONS->end()) return it->second;
    return 0;
}
void CPlayerObject::FreeBinocularOverlay() {
    if (binocularOverlay) {
        delete binocularOverlay;
        binocularOverlay = 0;
    }
}
