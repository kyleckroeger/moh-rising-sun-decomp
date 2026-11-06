// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "profile.h"
#include <map>
#include "CameraShake.h"
typedef _STL::map<int, CPlayerWeaponObject *> PlayerWeaponMap;
#define WEAPON_MAP (*(PlayerWeaponMap *)weaponMap)
int CPlayerObject::SetWeaponEnable(int slot, bool enable) {
    if (WEAPON_MAP.find(slot) != WEAPON_MAP.end()) {
        WEAPON_MAP[slot]->enabled = enable;
        return 1;
    }
    return 0;
}
