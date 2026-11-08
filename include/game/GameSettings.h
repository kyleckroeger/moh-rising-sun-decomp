// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_GAME_SETTINGS_H
#define GAME_GAME_SETTINGS_H

// Per-player progress record: 0xc598 bytes, first observed at +0xbf54 of the
// profile. Bit n of each mask is set for level n.
struct ProgressRecord {
    int filmCanisters[2]; // +0x0, +0x4; indexed by canister type.
    int replayItems;      // +0x8
    unsigned char unknown_c[0xc598 - 0xc];
};

// The per-player multiplayer entry; 20 bytes, the class tag is from InitMultiplayer's
// symbol. restartIndex (first byte, RestartPlayer's argument) is a descriptive name.
struct SMPPlayerData {
    unsigned char restartIndex;
    unsigned char unknown_1[15];
};
struct SMPPlayerSlot { int unknown_0; SMPPlayerData data; };

// Observed fields of the profile the settings object points to.
struct GameProfile {
    unsigned char unknown_0[0xbf54];
    ProgressRecord records[3];          // +0xbf54
    unsigned char unknown_3101c[0x31664 - 0x3101c];
    unsigned char mission;              // +0x31664
    unsigned char level;                // +0x31665
    unsigned char unknown_31666[0x32020 - 0x31666];
    SMPPlayerSlot players[4];           // +0x32024; indexed by the player's slot (count inferred).
    unsigned char unknown_32070[0x321e0 - 0x32070];
    signed char currentRecord;          // +0x321e0
};

// Original enum tag; members are descriptive.
enum EShotLocations { ShotLocation0 = 0 };

class CMatrix;
class CVector3;
class CQuaternion;
// Member view of the game-settings singleton; only the profile pointer is named.
class CGameSettings {
  public:
    GameProfile *profile;
    int SerializingIn() const;
    int SerializingOut() const;
    void SerializeDelta(const CMatrix &, CMatrix &);
    void SerializeValue(unsigned int &, unsigned int);
    void SerializeValue(CVector3 &);
    void SerializeValue(CQuaternion &);
    void AddCheat(int);
    void RegisterSavePoint(int, bool);
    void TriggerSavePointResponse(int, bool);
    int HasMedal(int) const;
    void Set_shotsFired(unsigned int, bool);
    void Set_hits(unsigned int, bool);
    void Set_hitsTaken(unsigned int, bool);
    void Set_enemiesKilled(unsigned int, bool);
    void Set_shotLocation(unsigned int, EShotLocations, bool);
};
extern CGameSettings g_GameSettings;

// Scoped view of the cheat state; only the replay-item unlock at +0x34 is named.
struct CheatsView {
    int invincible; // Tested by CPlayerObject::IsInvincible; name inferred.
    int infiniteAmmo; // +4; CWeapon::Update refills an empty reserve. Name from Frontline.
    unsigned char unknown_8[0x1c - 8];
    int keepGrenadeSpeed; // +0x1c; name from Frontline.
    unsigned char unknown_20[0x28 - 0x20];
    int oneHitKills; // +0x28; damage becomes 1000 in CExplosion::Visit. Name from Frontline.
    unsigned char unknown_2c[0x34 - 0x2c];
    int allReplayItems;
};
extern CheatsView g_cheats;

// Inline in the original: replay item 0 is always included in the mask.
inline bool CurrentRecordHasReplayItem(int item) {
    return (g_GameSettings.profile->records[g_GameSettings.profile->currentRecord].replayItems & ((1 << item) | 1)) != 0;
}

// Word-wide view of the profile bytes from +0x31664 (mission, level); the
// original stores the level-won bit with a full-word read-modify-write.
struct GameProfileLevelFlags {
    unsigned int mission : 8;
    unsigned int level : 8;
    unsigned int unknown : 6;
    unsigned int levelWon : 1;
};

// Observed field of the 0x14-byte game-flow global.
struct GameFlowView {
    unsigned char unknown_0[8];
    int levelEnding; // +0x8; set by BIFunc_EndLevel and CPauseMenu::RestartLevel.
    unsigned char unknown_c[0x14 - 0xc];
};
extern GameFlowView g_GameFlow;

#endif
