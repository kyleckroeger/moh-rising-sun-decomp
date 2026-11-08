// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "GameSettings.h"
#include "SceneObjects.h"
#include "Capsule.h"
#include "Box.h"
#include "BoxSetters.h"
#include "CLine3.h"
#include "Shell.h"
extern int g_bMPEndLevel;
extern int RESPAWN_FIRE_DELAY;
void RestartPlayer(CPlayerObject *, bool, unsigned char);
int BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
#define BOX (*(CVolBox *)boxStorage)
#define CAPSULE (*(CVolCapsule *)localVolume)
void CPlayerObject::Respawn() {
    if (g_bMPEndLevel) return;
    int none = -1;
    SetHealthMax();
    collisionId = (EClsnId)1;
    CAPSULE.radius = 0.39f;
    field_1000 = none;
    localBox = 0;
    worldBox = 0;
    field_270 = 1.1f;
    CAPSULE.line = CLine3(CVector3(0.0f, 0.0f, 0.0f), CVector3(0.0f, 0.0f, field_270));
    BOX.SetBasis(CVector3(1.0f, 0.0f, 0.0f), CVector3(0.0f, 1.0f, 0.0f), CVector3(0.0f, 0.0f, 1.0f));
    BOX.SetWidth(0.75f);
    BOX.SetHeight(0.75f);
    BOX.SetDepth(0.6f);
    BOX.SetCenter(CVector3(0.25f, 0.7f, -0.1f));
    CPlayerWeaponObject *previous = currentWeapon;
    fireDelay = RESPAWN_FIRE_DELAY;
    pathBits.climbCleared = 0;
    pathBits.bit26 = 0;
    flags.motionShake = 0;
    pathBits.meleeImmune = 0;
    field_e88 = 0;
    pathBits.crouching = 0;
    pathBits.bit11 = 0;
    flags.canCrouch = 1;
    currentWeapon = 0;
    flags.flag24 = 0;
    flags.pitchWhileMounted = 1;
    flags.hideWeapon = 0;
    pathBits.climbing = 0;
    pathBits.bit19 = 0;
    flags.wielding = 0;
    pathBits.crouched = 0;
    flags.triggerChecks = 1;
    flags.flag27 = 1;
    flags.fallingDamage = 1;
    pathBits.bit28 = 1;
    flags.superAimMode = 0;
    pathBits.mmgBlocked = 0;
    fov = 35.0f;
    field_e10 = 0.0f;
    field_10c8 = 1.0f;
    field_d50 = 0.0f;
    targetFov = 35.0f;
    mountedWeapon = 0;
    field_112c = 0;
    pathBits.noShells = 0;
    field_1130 = 0.0f;
    field_1134 = 0.0f;
    field_1138 = 0.0f;
    field_113c = 0.0f;
    field_1140 = 0;
    fovRate = 0.0f;
    peekBase = CVector3(0.0f, 0.0f, 0.0f);
    pathBits.peeking = 0;
    pathBits.bit18 = 0;
    flags.flag13 = 0;
    flags.flag12 = 0;
    flags.invincible = 0;
    field_e58 = 0.0f;
    field_e5c = 0.0f;
    field_10c2 = 0;
    flags.headingOverride = 0;
    recoilYaw = 0.0f;
    recoilPitch = 0.0f;
    surfaceSound = 40;
    field_10d0 = none;
    field_10c4 = 0.0f;
    pitch = 0.0f;
    g_inputMgr.SetBlockVibrations(false, playerIndex);
    SMPPlayerSlot *slots = g_GameSettings.profile->players;
    SMPPlayerData *data = &slots[playerIndex].data;
    InitMultiplayer(data, true);
    if (previous && currentWeapon != previous) CycleWeapon(1, previous->weapon->crc);
    RestartPlayer(this, false, data->restartIndex);
    BSObjectTriggerEvent(script, 132, 0, 0, false);
    field_d54 = 119.879990f;
}
