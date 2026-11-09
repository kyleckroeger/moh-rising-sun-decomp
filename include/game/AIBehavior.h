// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_AI_BEHAVIOR_H
#define GAME_AI_BEHAVIOR_H
#include "AITargeting.h"
class CThrownBullet;
class TriggerObject;
class CStaticObject;

// Method-only views of the behaviors that script built-ins reach through
// CAIObject::FindMatchingBehavior. Each class's GetType returns the selector
// noted beside it. Storage, bases and virtual interfaces are not declared; the
// built-ins convert the returned behavior pointer without adjustment.
// Observed prefix of a buddy-lead checkpoint record.
struct BuddyLeadCheckpoint {
    int unknown_0;
    int value; // +0x4; returned by BIFunc_GetBuddyLeadCheckpointFromID.
};
class CBuddyLeadBehavior { // Type 1.
  public:
    static BuddyLeadCheckpoint *GetBuddyLeadCheckpointFromID(int);
    void HandleCheckpointReachedByBuddy(int);
    static void HandleCheckpointReachedByPlayer(int);
};

class CAIActivityZone {
  public:
    void Disable();
    void UseMovingTarget(ISceneNode *, float);
};

class CBuddyFollowBehavior : public CBehavior { // Type 2.
  public:
    CBuddyFollowBehavior(CAIActivityZone *);
    virtual ~CBuddyFollowBehavior();
    virtual void Enable();
    virtual void Disable();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    void BumpPlayer();
    void CantReachTarget();
    void FinishedMovingAway();
    void SetDistances(float, float, float, float);
    void EnterBuddyTeleportArea(int);
    void ExitBuddyTeleportArea(int);
    int IsBuddyInArea(int);
    float UpdateDistanceToPlayer();
    float GetStopDistance();
    float GetStartDistance();
    static void RegisterBuddyFollowObject(BSObject *, float, float, float, float);
    float heuristicScore;           // +0x10
    float stopDistance;             // +0x14; state 4 uses stopAttackDistance instead.
    float moveThreshold;            // +0x18; state 4 uses breakOffAttackDistance instead.
    float breakOffAttackDistance;   // +0x1c
    float stopAttackDistance;       // +0x20
    int stopped;                    // +0x24; set by StopBehavior, cleared by StartBehavior.
    int cantReachTarget;            // +0x28
    float cantReachExpireTime;      // +0x2c
    int distancesSet;               // +0x30; keeps Enable from restoring the defaults.
    CAIActivityZone *activityZone;  // +0x34
    int teleportArea;               // +0x38
    int teleportAreaCount;          // +0x3c
    int movingAway;                 // +0x40; set by BumpPlayer.
    float movingAwayExpireTime;     // +0x44
    static BSObject *m_pBuddyFollowObject;
    static float m_defaultStopDistance;
    static float m_defaultMoveThreshold;
    static float m_defaultBreakOffAttackDistance;
    static float m_defaultStopAttackDistance;
};

class CPistolBehavior : public CBehavior { // Type 6.
  public:
    CPistolBehavior();
    virtual ~CPistolBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual BSObject *GetScriptObject();
    virtual void SetWeaponScript(BSObject *);
    EAIHighLevelState highLevelState; // +0x10; passed with the start and stop events.
    float heuristicScore; // +0x14
    BSObject *weaponScript; // +0x18
};

class CRifleBehavior : public CBehavior { // Type 7.
  public:
    CRifleBehavior();
    virtual ~CRifleBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual BSObject *GetScriptObject();
    virtual void SetWeaponScript(BSObject *);
    EAIHighLevelState highLevelState; // +0x10; passed with the start and stop events.
    float heuristicScore; // +0x14
    BSObject *weaponScript; // +0x18
};

class CNullWeaponBehavior : public CBehavior { // Type 9.
  public:
    CNullWeaponBehavior();
    virtual ~CNullWeaponBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual BSObject *GetScriptObject();
    virtual void SetWeaponScript(BSObject *);
    EAIHighLevelState highLevelState; // +0x10; passed with the start and stop events.
    float heuristicScore; // +0x14
    BSObject *weaponScript; // +0x18
};

class CPolemineBehavior : public CBehavior { // Type 7, as the rifle.
  public:
    CPolemineBehavior();
    virtual ~CPolemineBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual BSObject *GetScriptObject();
    virtual void SetWeaponScript(BSObject *);
    float heuristicScore; // +0x10
    BSObject *weaponScript; // +0x14
};

class CMountedMachineGunBehavior { // Type 3.
  public:
    // The table pointer follows twelve bytes of base data; slots 1-4 in the
    // order of the original behavior tables (e.g. 0x802e8180).
    unsigned char unknown00[12];
    virtual ~CMountedMachineGunBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual float GetHeuristicUpdateFrequency();
    virtual void UpdateHeuristicScore();
    void Arrived();
    void CantReachMMG();
    void HitFromBehind();
    void ForceToMMG();
    void ForceToMMGFeeder();
    static void DeregisterGun(int);
    static int GetFeederByLinkCode(int);
    static CStaticObject *GetGunByLinkCode(int);
    static void RegisterFeeder(int, BSObject *);
    static void RegisterGun(int, CStaticObject *);
};

class CCoverpointBehavior { // Type 4.
  public:
    // The table pointer follows twelve bytes of base data; slots 1-4 in the
    // order of the original behavior tables (e.g. 0x802e8180).
    unsigned char unknown00[12];
    virtual ~CCoverpointBehavior();
    static void UpdateCoverpointsOutOfArea(const ISceneNode *);
    virtual float GetCurrentHeuristicScore();
    virtual float GetHeuristicUpdateFrequency();
    virtual void UpdateHeuristicScore();
    int StartCoverPointWalk();
    void WalkToCoverPoint();
    float GetAIValueByName(const char *);
    void StopCoverPointWalk();
    void Arrived();
    void EndCoverpoint();
    void ForceToCoverPoint();
    static void InvalidateCoverPointByID(int);
    static void RegisterCoverpoint(TriggerObject *);
};

class CScriptedBehavior : public CBehavior { // Type 5.
  public:
    CScriptedBehavior();
    virtual ~CScriptedBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual void ReleaseResources();
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual BSObject *GetScriptObject();
    void SetAllowedHighLevelState(EAIHighLevelState);
    void SetCapability(ECapability, bool);
    void StartScript(BSObject *);
    void StopScript();
    BSObject *script;        // +0x10
    BSObject *pendingScript; // +0x14; started when StopBehavior ends the current script.
    int scriptActive;        // +0x18; makes the behavior score at all.
    int scoreChanged;        // +0x1c; read and cleared by UpdateHeuristicScore.
    int capabilities[11];    // +0x20; indexed by ECapability.
    int scriptedType;        // +0x4c; read and written by the scripted-behavior type built-ins.
    int allowedStateMask;    // +0x50; bit per high-level state; 0xffff allows every state.
};

class CSwordBehavior : public CBehavior { // Type 8.
  public:
    CSwordBehavior();
    virtual ~CSwordBehavior();
    virtual float GetCurrentHeuristicScore();
    virtual int GetHeuristicUpdateFrequency();
    virtual int UpdateHeuristicScore();
    virtual int StartBehavior();
    virtual void StopBehavior(bool);
    virtual int GetCapabilities(ECapability);
    virtual int GetType();
    virtual unsigned int GetCRC();
    virtual BSObject *GetScriptObject();
    virtual void SetWeaponScript(BSObject *);
    int IsSwordDrawn();
    void SetSwordDrawn(bool);
    int swordDrawn; // +0x10
    float heuristicScore; // +0x14
    BSObject *weaponScript; // +0x18
};

class CGrenadeResponseBehavior { // Type 10.
  public:
    unsigned char unknown_0[0x24];
    int caught; // +0x24; set by BIFunc_CatchGrenade.
    void FinishedResponse(CThrownBullet *);
};

// Static scripted-gag entry points.
class CScriptedGag {
  public:
    static void RegisterGagAction(BSObject *, const char *, int);
    static void StartGag(BSObject *, const char *, int);
    static void StartGag(BSObject *, unsigned int, int);
};

// A cover point; the script object's native trigger is one. Its first word holds the state
// bits below; their names follow the accessors.
class CCoverPoint {
  public:
    void InitFlags();
    int IsOccupied() const;
    int IsValid() const;
    int IsAvailable() const;
    int FlagAsOccupied();
    void FlagAsUnoccupied();
    void SetOutOfArea(bool);
    void Invalidate();

    unsigned int unknownHigh : 16;
    unsigned int unknownBits : 13;
    unsigned int outOfArea : 1;
    unsigned int invalid : 1;
    unsigned int occupied : 1;
};

// Static lookup used by BIFunc_AIFindTank; script object at +0x0c as in CAIObject.
class CAITankObject {
  public:
    unsigned char unknown00[0x0c];
    BSObject *scriptObject;
    unsigned char unknown10[0x730 - 0x10];
    float health;      // +0x730
    float maxHealth;   // +0x734
    aistatus_match targetMatchList[1]; // +0x738; extent unknown.
    unsigned char unknown744[0x768 - 0x744];
    int field_768;     // +0x768; cleared by Init.
    unsigned char unknown76c[0x7a0 - 0x76c];
    ISceneNode *sceneNode; // +0x7a0; the tank scene object.
    unsigned char unknown7a4[1968 - 0x7a4]; // The allocation is 1968 bytes.
    static CAITankObject *FindTank();
    CAITankObject(void *);
    void *operator new(unsigned int, void *);
    void operator delete(void *);
    void Init();
    void SetInitialStatus();
    void UpdateStatus();
    void UpdateFromGameObject(IMovingSceneNode *);
    void PreUpdate();
    void SetupTargetMatchList(aistatus_match *, int);
    int ChooseTarget(float);
    ISceneNode *GetSceneNode();
    CAITankObject *AsAITankObject();
};

#endif
