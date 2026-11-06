// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "profile.h"
#include <map>
#include "CameraShake.h"
#include "SceneObjects.h"
typedef _STL::map<int, CPlayerWeaponObject *> PlayerWeaponMap;
// CPlayerObject's view keeps the map as raw storage so that other users need no STL headers.
#define WEAPONS ((const PlayerWeaponMap *)weaponMap)
#define MUTABLE_WEAPONS ((PlayerWeaponMap *)weaponMap)
extern int g_bInMultiplayerMode;
extern int g_bCoopMultiplayer;
void CPlayerObject::Reset() {
    localToWorld.GetSlot(0);
    cameraMatrix.GetSlot(0);
    acceleration.Zero();
    velocity.Zero();
    position.Zero();
    field_10cc = 1.0f;
    pathFlags &= 0x9fffffff;
    field_270 = 1.1f;
    yaw = 0.0f;
    pitch = 0.0f;
    field_288 = 0.0f;
    field_284 = 0.0f;
    field_dc0 = 0.0f;
    field_d40 = 0.0f;
    field_28c = 0.0f;
    field_10a0.Zero();
    currentWeapon = 0;
    field_d48 = 0;
    MUTABLE_WEAPONS->clear();
    CPlayerWeaponObject::m_pPlayerWeaponFirst = 0;
    if ((g_bInMultiplayerMode || g_bCoopMultiplayer) && body) ((CSoldierObject *)body)->RemoveWeapons();
    field_e8c = 0;
    field_e90 = 0;
    turretWeapon = 0;
    lookAt.Disable();
    targetFind.Disable();
    cameraShake.SetShake(0.0f, 0.0f);
    backgroundShake.SetShake(0.0f, 0.0f);
}
