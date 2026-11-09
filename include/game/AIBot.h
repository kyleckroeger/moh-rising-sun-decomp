// AI-assisted reconstruction from GR8E69. Member views of the multiplayer bot classes:
// method and class names come from the symbol table, member names and offsets from
// their use in the reconstructed functions. This is not a complete class layout; see
// docs/Behaviors.md.
#ifndef GAME_AI_BOT_H
#define GAME_AI_BOT_H
#include "AITargeting.h"
class CWeapon;
// A value-by-input graph; only the lookup is known.
class CGraph {
  public:
    float GetValue(float);
};
// One entry of the bot's weapon table (40 bytes, from +0x1a38); the fields are inferred.
struct BotWeaponEntry {
    CGraph rangeGraph; // +0x00; looked up at 0 and 1 for the weapon's minimum and maximum range.
    unsigned char unknown01[0x0c - 0x01];
    CWeapon *weapon;   // +0x0c
    unsigned char unknown10[40 - 0x10];
};
struct BotCampingPoint {
    unsigned char unknown00[48];
};
struct EnemyInfo {
    unsigned char unknown00[80];
};
class CBotObject;
class CBotMan {
  public:
    CBotObject *enemies[1];            // +0x000; indexed by GetEnemyAI, extent not established.
    unsigned char unknown004[0x2e0 - 4];
    BotCampingPoint campingPoints[1];  // +0x2e0; extent not established.
    unsigned char unknown310[0x1a2c - 0x310];
    float clock;                       // +0x1a2c
};
extern CBotMan g_BotMan;
// The bot derives from the soldier object; only the members its functions touch are named.
class CBotObject {
  public:
    float GetClock();
    CBotObject *GetEnemyAI(unsigned int);
    struct EnemyInfo &GetEnemyInfo(unsigned int);
    struct EnemyInfo *GetpEnemyInfo(unsigned int);
    int EnemyInView();
    float GetHealth();
    char *GetName();
    unsigned char GetPlayerIndex();
    unsigned char GetTeam();
    float GetHealthPercent();
    void SetLookTarget(const CVector3 &);
    void SetMoveTarget(const CVector3 &);
    void LookAlongPath();
    int FollowingAStar();
    void StopAStar();
    int WithinGrenadeBlastRadius();
    void *GetCurrentPathNode();
    BotCampingPoint *GetCurrentCampingPoint();
    void SetPlayerIndex(unsigned char);
    void SetName(const char *);
    int CanFireWeapon();
    void StartReload();
    void FireAtEnemy();
    void Jump();
    void *GetGrenade();
    float GetMinWeaponRange();
    float GetMaxWeaponRange();
    void ClearEvents();
    void CheckTrigger();
    unsigned char unknown0000[0x120];
    BSObject *scriptObject;  // +0x120
    unsigned char unknown0124[0xf20 - 0x124];
    int currentWeaponIndex;  // +0xf20
    unsigned char unknown0f24[0x15a8 - 0xf24];
    CAIObject *aiObject;     // +0x15a8
    unsigned char unknown15ac[0x16d0 - 0x15ac];
    int eventCount;          // +0x16d0
    int eventIndex;          // +0x16d4
    unsigned char unknown16d8[0x1790 - 0x16d8];
    EnemyInfo enemyInfo[1];  // +0x1790; extent not established.
    unsigned char unknown17e0[0x1a10 - 0x17e0];
    int *enemyInView;        // +0x1a10; its first word is tested by EnemyInView.
    unsigned char unknown1a14[0x1a38 - 0x1a14];
    BotWeaponEntry weapons[1]; // +0x1a38; extent not established.
    unsigned char unknown1a60[0x1b1c - 0x1a60];
    void *grenade;           // +0x1b1c
    unsigned char unknown1b20[4];
    int moveState;           // +0x1b24
    unsigned char unknown1b28[0x1b40 - 0x1b28];
    CVector3 lookTarget;     // +0x1b40
    CVector3 moveTarget;     // +0x1b50
    unsigned char unknown1b60[0x1c50 - 0x1b60];
    float fireDelay;         // +0x1c50
    float reloadEndTime;     // +0x1c54
    unsigned char unknown1c58[0x1c68 - 0x1c58];
    int fireAtEnemy;         // +0x1c68
    int lookAlongPath;       // +0x1c6c
    int jump;                // +0x1c70
    unsigned char unknown1c74[0x1cb0 - 0x1c74];
    float grenadeDistanceSquared; // +0x1cb0
    unsigned char unknown1cb4[0x1ce0 - 0x1cb4];
    int currentCampingPoint; // +0x1ce0
    unsigned char unknown1ce4[0x1e8c - 0x1ce4];
    float lookTargetExpireTime; // +0x1e8c
    unsigned char unknown1e90[4];
    unsigned char playerIndex; // +0x1e94
    char name[1];            // +0x1e95; extent not established.
};
#endif
