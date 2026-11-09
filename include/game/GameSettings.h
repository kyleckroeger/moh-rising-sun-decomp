// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_GAME_SETTINGS_H
#define GAME_GAME_SETTINGS_H

// Class and struct tags SPlayerStats, SSaveData, SCRC_Ammo, SPlayerProfile, SCoreGameSettings
// and CMovieSettings come from method symbols. Member names are descriptive unless a method
// symbol names them (shotsFired, hits, hitsTaken, enemiesKilled, shotLocation).
// Each struct holds the members that reconstructed code touches, at the observed offsets.

struct SCRC_Ammo {
    int weaponId;          // +0x0
    unsigned short ammoA;  // +0x4
    unsigned short ammoB;  // +0x6
    unsigned int selectable : 1; // +0x8, top bit
    SCRC_Ammo() { Reset(); }
    void Reset() { weaponId = 0; ammoA = 0; selectable = 0; ammoB = 0; }
};

struct SPlayerStatsEntry {
    int value;           // +0
    unsigned short count; // +4
    SPlayerStatsEntry() : value(0), count(0) {}
};

struct SPlayerStats {
    unsigned short shotsFired;      // +0x00
    unsigned short hits;            // +0x02
    unsigned short unknown_4[2];    // +0x04
    unsigned short hitsTaken;       // +0x08
    unsigned short unknown_a;       // +0x0a
    unsigned short enemiesKilled;   // +0x0c
    unsigned short shotLocation[7]; // +0x0e; indexed by EShotLocations.
    int levelTimer;                 // +0x1c; UpdateLevelTimer adds to it.
    SPlayerStatsEntry entries[8];   // +0x20
    int dirty;                      // +0x60; set by every Set_ stat method.
    SPlayerStats() { Reset(); }
    void Reset();
};

// Bit-packed store for SerializeValue; 0x800 bytes per its position in SSaveData.
class BitBuffer {
  public:
    unsigned int Load(unsigned int);
    void Save(unsigned int, unsigned int);
    unsigned char storage[0x800];
};

// Named from the field widths: 16, 4, 6, 5, 6 and 6 bits.
struct SSaveTime {
    unsigned short year;     // +0x0
    unsigned int month : 4;  // +0x2
    unsigned int day : 6;
    unsigned int hour : 5;
    unsigned int minute : 6; // +0x4
    unsigned int second : 6;
    SSaveTime() { int zero = 0; year = zero; month = zero; day = zero; hour = zero; minute = zero; second = zero; }
};

// The level state kept in the shared settings and in each profile's save slots.
struct SSaveData {
    unsigned char mission;           // +0x000
    unsigned char level;             // +0x001
    unsigned int savePointID : 6;    // Top six bits of the half word at +0x002.
    unsigned int levelWon : 1;       // Mask 0x200 of the word at +0x000.
    unsigned int unknownBits : 8;
    int ambientTrack;                // +0x004
    int oneShotBank;                 // +0x008
    int musicEvent;                  // +0x00c
    int currentWeapon[2];            // +0x010; indexed by player.
    SCRC_Ammo ammo[2][8];            // +0x018; indexed by player, then by slot.
    float health[2];                 // +0x0d8; indexed by player.
    SPlayerStats stats;              // +0x0e0
    int unknown_144;                 // +0x144
    unsigned char compartmentID[4];  // +0x148; indexed by player.
    BitBuffer buffer;                // +0x14c; holds the serialized scene.
    SSaveTime saveTime;              // +0x94c
    SSaveData();
    void TotalReset();
    void LevelReset();
};

// One stored save profile; 0xc598 bytes. The bit masks use bit n for level n.
struct SPlayerLevelData {
    unsigned char mission;         // +0
    unsigned char level;           // +1
    unsigned int finished : 1;     // +2, top bit.
    float health;                  // +4
};

struct SPlayerProfile {
    char name[12];                   // +0x0000
    int device;                      // +0x000c; saved by RememberProfileDevices.
    unsigned char controllerSetting; // +0x0010
    unsigned char difficulty;        // +0x0011
    unsigned char unknown_12[2];
    float volumeA;                   // +0x0014; first SoundSetVolumes argument.
    float volumeB;                   // +0x0018; second SoundSetVolumes argument.
    unsigned char soundMode;         // +0x001c
    unsigned char unknown_1d[0x2c - 0x1d];
    unsigned int unknownBits : 4;    // +0x002c; the upper half of this word holds option bits.
    unsigned int hudOption : 1;      // InitHUDFromOptions copies it to the player.
    unsigned int saveNamed : 1;      // Tested before the save screen; the keyboard popup runs otherwise.
    unsigned int optionBits : 10;
    short buttons[2][14];            // +0x002e
    unsigned char unknown_66;
    unsigned char prevMission;       // +0x0067
    unsigned char prevLevel;         // +0x0068
    unsigned char currentSaveSlot;   // +0x0069
    unsigned char unknown_6a[2];
    SSaveData saves[20];             // +0x006c
    unsigned char unknown_bafc[0xbf48 - 0xbafc];
    int medals;                      // +0xbf48
    int unlockedLevels;              // +0xbf4c
    int filmCanisters[2];            // +0xbf50; indexed by canister type.
    int replayItems;                 // +0xbf58
    int cheats;                      // +0xbf5c
    int activeCheats;                // +0xbf60
    unsigned char coopDifficulty;    // +0xbf64
    unsigned char unknown_bf65;
    unsigned char coopPrevLevel;     // +0xbf66
    unsigned char unknown_bf67[0xc3b4 - 0xbf67];
    int coopMedals;                  // +0xc3b4
    int coopUnlockedLevels;          // +0xc3b8
    int coopCheats;                  // +0xc3bc
    unsigned char unknown_c3c0[0xc4e8 - 0xc3c0];
    SPlayerLevelData levelData[22];  // +0xc4e8
    void TotalReset();
};

// The per-player multiplayer entry; 20 bytes. restartIndex (first byte, RestartPlayer's
// argument) is a descriptive name; SMPPlayerData is from InitMultiplayer's symbol.
struct SMPPlayerData {
    unsigned char restartIndex; // +0x00; InitMultiplayer's team.
    char bodyIndex;     // +0x01
    char name[11];      // +0x02
    unsigned char maxHealth;    // +0x0d; SetHealthMax's value.
    unsigned char unknown_e[2];
};
struct SMPPlayerSlot { int unknown_0; SMPPlayerData data; };

struct SDMStats {
    int unknown[6];
};

// Saved game data the settings object points to; Init reports sizeof as 0x321ec of 0x321f8 reserved.
struct SCoreGameSettings {
    int language;                       // +0x00000
    SPlayerProfile profiles[4];         // +0x00004
    SSaveData saveData;                 // +0x31664
    SPlayerStats levelStats;            // +0x31fb8
    unsigned char unknown_3201c[0x32020 - 0x3201c];
    SMPPlayerSlot players[4];           // +0x32020; indexed by the player's slot (count inferred).
    int unknown_32070;                  // +0x32070
    int field_32074;                    // +0x32074; a team setting CPlayerObject::HandleBulletCollision requires non-negative for friendly fire protection.
    SDMStats dmStats[8];                // +0x32078
    int unknown_32138[2];               // +0x32138
    unsigned char unknown_32140[0x321e0 - 0x32140];
    signed char profileIndex[4];        // +0x321e0; indexed by player.
    signed char unknown_321e4;          // +0x321e4
    unsigned char unknown_321e5;
    unsigned char unknown_321e6;
    unsigned char unknown_321e7;
    int wasLastGameSinglePlayer;        // +0x321e8
    void TotalReset();
    void ResetDMStats();
};

// Original enum tag; members are descriptive.
enum EShotLocations { ShotLocation0 = 0 };

class CMatrix;
class CVector3;
class CQuaternion;
// CMovieSettings: the per-level movie list. Only its reset fields are named.
class CConfigFile;
class CPlayerObject;
class CMovieSettings {
  public:
    bool movieLoad;             // +0x000; the fields are named by their game.cfg keys.
    int movieMemory;            // +0x004
    char introMovie[64];        // +0x008
    char introText[64];
    char introTime[64];
    char outroMovie[64];
    char outroText[64];
    char outroTime[64];
    char creditsMovie[64];
    char creditsText[64];
    char creditsTime[64];
    char historicalMovie[64];
    char historicalText[64];
    char historicalTime[64];
    char letterMovie[64];
    char letterText[64];
    char letterTime[64];
    char movieFont[64];         // +0x3c8
    char movieBack[64];         // +0x408
    CMovieSettings();
    void Reset();
    void Load(CConfigFile &, int, int);
    void LoadSection(CConfigFile &, const char *);
};

// Member view of the game-settings singleton.
class CWeapon;
class IMovingSceneNode;
struct KEYMAP;
enum ELanguages { LanguageUnknown0 = 0 };
enum ESoundMode { ESoundMode0 = 0 };
class CGameSettings {
  public:
    CGameSettings();
    void PerformTotalReset(const char *);
    void PerformTotalReset();
    void Init();
    void InvalidateSlotBackup();
    void PerformLevelReset();
    void SetPrevMissionAndLevel(unsigned int, unsigned int);
    void SetNextMissionAndLevel(unsigned int, unsigned int, int);
    void SetCurrentMissionAndLevel(unsigned int, unsigned int);
    void Set_Language(ELanguages);
    void ResetCurrentLevelStats();
    void InitCheatsFromOptions();
    void SetPlayersFinishedLevelSuccessfully(int, int);
    int GetPlayersFinishedLevelSuccessfully(int, int);
    void SetCurrentHealth(float, int);
    float GetCurrentHealth(int) const;
    void SetCurrentWeapon(CWeapon *, int);
    int GetCurrentWeapon(int &, int);
    void SetCurrentAmmo(int, unsigned int, unsigned int, int);
    void GetCurrentAmmo(int, unsigned int &, unsigned int &, int) const;
    void SetIfCurrentWeaponSelectable(int, bool, int);
    int GetIfCurrentWeaponSelectable(int, int) const;
    void SetPlayersFinishedLevelHealth(int, int, float);
    void ActivateCheatNamed_All();
    void ActivateCheatNamed_Unlock();
    void ActivateCheatNamed_HugeArms();
    void AddMedal(int);
    void ResetLevelTimer();
    void UpdateLevelTimer(CPlayerObject *, unsigned int);
    void SaveData();
    void LoadData();
    void SetControllerButtons(int, int, short *, int);
    void GetControllerButtons(int, int, short *, int);
    void CopyNameIntoSettings(const char *, int);
    char *GetNameFromSettings(int);
    int Get_PlayerControllerSetting(int);
    void InitHUDFromOptions(int);
    void InitAudioFromOptions(int, bool);
    int GetSoundModeValue(int);
    void SetSoundModeValue(int, ESoundMode);
    int CurrentLevelIsShell() const;
    void BindKeyToEvent(KEYMAP *, unsigned short, unsigned short);
    void SetUpButtonConfig(int, short *);
    int PlayerLevelDataIndex(int, int);
    void RestoreSaveData();
    void RestoreSlotBackup();
    void RememberProfileDevices();
    void RestoreProfileDevices();
    void RestoreLevelAudio();
    void RestoreCoreLevelStats();
    void StoreSaveData(bool);
    void Validate();
    SSaveData *GetSaveData(unsigned int, unsigned int) const;
    void SetCurrentCompartmentID(unsigned int, int);
    int GetCurrentCompartmentID(unsigned int) const;
    int GetCurrentOneShotBankID();
    int GetCurrentSavePointID() const;
    int HasBeenUnlocked(int) const;
    void UnlockLevel(int, bool);
    void Set_currentDifficulty(unsigned char);
    int HasCheat(int) const;
    int IsCheatActive(int) const;
    void ActivateCheat(int, bool);
    int Get_currentDifficulty() const;
    int GetProfilesCRCValue();
    int GetProfileIndexFromPlayerID(int);
    int GetPlayerIDFromProfileIndex(int);
    int WasLastGameSinglePlayer();
    SPlayerProfile *GetProfile(unsigned int);
    void PerformSavePointResponse();
    void InitializeFinishedLevelData(int);

    SCoreGameSettings *profile; // +0x000
    int profileDevice[4];       // +0x004; RememberProfileDevices' copy.
    int nextMission;            // +0x014
    int nextLevel;              // +0x018
    const char *languageCode;   // +0x01c
    int languageIndex;          // +0x020
    int loadedData;             // +0x024; from the loader's +0x54.
    unsigned char unknown_28[0xa8 - 0x28];
    int unknown_a8; // Read as a copy by the pop-up message handler. Set to 1 by the constructor and total reset.
    int unknown_ac;
    int unknown_b0; // Compared with 1 before the language is used for loading screens.
    int slotBackupSlot;         // +0x0b4
    int slotBackupPlayer;       // +0x0b8
    int serializeMode;          // +0x0bc; 1 or 3 write, 2 reads.
    int savePointPending;       // +0x0c0
    int savePointSoft;          // +0x0c4
    SSaveData levelStartData;   // +0x0c8
    int slotBackupValid;        // +0xa1c
    SSaveData slotBackup;       // +0xa20
    CMovieSettings movies;      // +0x1374
    int SerializingIn() const;
    int SerializingOut() const;
    void SerializeDelta(const CMatrix &, CMatrix &);
    void SerializeValue(unsigned int &, unsigned int);
    void SerializeValue(int &, unsigned int);
    void SerializeValue(unsigned char &);
    void SerializeValue(bool &);
    void SerializeValue(float &);
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
    int oneShotKillsEnemies; // +0x20; a soldier's bullet damage becomes 1000. Descriptive.
    int headShotsOnly;       // +0x24; name from Frontline.
    int oneHitKills; // +0x28; damage becomes 1000 in CExplosion::Visit. Name from Frontline.
    unsigned char unknown_2c[0x30 - 0x2c];
    int hugeArms; // +0x30; set by ActivateCheatNamed_HugeArms.
    int allReplayItems;
};
extern CheatsView g_cheats;

// Inline in the original: replay item 0 is always included in the mask.
inline bool CurrentRecordHasReplayItem(int item) {
    return (g_GameSettings.profile->profiles[g_GameSettings.profile->profileIndex[0]].replayItems & ((1 << item) | 1)) != 0;
}

// Observed field of the 0x14-byte game-flow global.
struct GameFlowView {
    unsigned char unknown_0[8];
    int levelEnding; // +0x8; set by BIFunc_EndLevel and CPauseMenu::RestartLevel.
    unsigned char unknown_c[0x14 - 0xc];
};
extern GameFlowView g_GameFlow;

#endif
