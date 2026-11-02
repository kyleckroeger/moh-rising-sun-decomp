// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SCENE_OBJECTS_H
#define GAME_SCENE_OBJECTS_H
#include "SceneNode.h"
#include "AITargeting.h"
#include "CMatrix.h"
#include "CQuaternion.h"
#include "LightVolumeManager.h"
#include "CVector3.h"
struct BSObject;
class IVolume;
class CWeapon;
// Original enum tag; its members are unknown.
enum EBSEventEnum { BSEventUnknown = 0 };

// Original enum tag; its members are unknown.
enum EPlayerCollisionType { PlayerCollisionTypeUnknown = 0 };
EClsnId GetCollisionIDFromScriptCollisionType(EPlayerCollisionType);
enum EScriptCollisionType { ScriptCollisionTypeUnknown = 0 };
EClsnId GetCollisionIDFromScriptCollisionType(EScriptCollisionType, ETeam);
// Original enum tag; its members are unknown.
enum EFireMethod { FireMethodUnknown = 0 };

// Member-only views of scene-object classes reached through the ISceneNode
// casts. Each view starts at the ISceneNode subobject (the casts return it
// unadjusted) and names only the observed members; bases, virtual interfaces
// and complete sizes are not declared. Built-ins that need an inherited virtual
// convert the view back to ISceneNode without adjustment.
// The soldier's embedded jaw controller. Member names are descriptive inferences
// from the reconstructed methods.
enum EJawType { JawTypeUnknown = 0 };
class CJawController {
  public:
    CJawController();
    void Update(float);
    void SetMotionType(EJawType);
    float closeStart; // +0x00; timer value at which the closing phase starts.
    float closeEnd;   // +0x04; timer value above which the jaw is at rest.
    float riseTime;   // +0x08
    float holdTime;   // +0x0c
    float fallTime;   // +0x10
    int cyclesLeft;   // +0x14
    int cycleCount;   // +0x18
    float minValue;   // +0x1c
    float maxValue;   // +0x20
    int previousCode; // +0x24; meaning unknown.
    int currentCode;  // +0x28; meaning unknown.
    float timer;      // +0x2c
    float value;      // +0x30
    float startDelay; // +0x34
    int delaying;     // +0x38
    int type;         // +0x3c
};

struct MotionData;
MotionData *InitMotionData(const char *);
class CBullet;
class CLight;
class CCollision;
class CStaticMesh;
class CTankObject;
struct MotionFrame;
// Method-only view; the storage it occupies in an object is not established.
class CObjectFadeManager {
  public:
    void StartFadeOut(float, float);
    void StartFadeIn(float, float);
    float FadeValue() const;
};
class CStaticObject {
  public:
    void SetLocalBoundingVolume(ISceneNode::EVolumeType, IVolume *);
    void SetWorldBoundingVolume(ISceneNode::EVolumeType, IVolume *);
    IVolume *GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    IVolume *GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    void InitMotionProperties(CStaticMesh *, int, int, int, int, int, MotionFrame *);
    void Init();
    float CalculateMovementTime(const CVector3 &, const CVector3 &);
    void ScaleFromScript(float, float, EBSEventEnum);
    void CommitAnimMotionUpdate();
    void CommitPathMotionUpdate();
    void UpdatePathMotionDestAndSpeed();
    void Destroy();
    void TriggerScriptEvent(unsigned short, void *, bool);
    void InitAim(bool, float);
    float FadeValue() const;
    void SerializeSaveData();
    void SetScript(BSObject *);
    void SetCollisionId(EClsnId);
    void SetDrawEnabled(bool);
    int GetObjectType() const;
    CVector3 GetWorldLinearVelocity() const;
    CStaticObject *AsStaticObject();
    const CStaticObject *AsStaticObject() const;
    BSObject *GetScriptObject() const;
    unsigned int IsDrawEnabled() const;
    void SetLightVolumeTransitionDuration(float);
    void *AsThrownObject();
    void *AsWeaponObject();
    CLight *GetAttachedLight() const;
    CTankObject *AsHierTankObject();
    EClsnId GetCollisionId() const;
    void GetTMLocalToWorld(CMatrix &) const;
    void GetPosition(CVector3 &) const;
    void GetRightward(CVector3 &) const;
    void GetForward(CVector3 &) const;
    void GetUpward(CVector3 &) const;
    void SetTMLocalToWorld(const CMatrix &);
    void SetPosition(const CVector3 &);
    void SetBasis(const CVector3 &, const CVector3 &, const CVector3 &);
    void EnterLightVolume(BPDLightVolume *);
    void ExitLightVolume(BPDLightVolume *);
    BPDLightVolume *GetLightVolume();
    void MarkForDestruction(int);
    void Draw(CDrawContext &);
    void BeginUpdate(float);
    int IsVisible(CDrawContext &) const;
    void AttemptUpdate(float);
    void CommitUpdate();
    void StopMotionPlayback();
    void ModifyMotionData(MotionData *);
    void SetMovementPath(int, bool, bool, float, float, float, float, BSObject *, float, bool);
    void ResumeMotionPlayback();
    void PlayAnimation(int, int, int, EBSEventEnum);
    void FadeIn(float);
    void FadeOut(float);
    struct Flags {
        unsigned int drawEnabled : 1;   // Bit 31.
        unsigned int scriptMoving : 1;  // Bit 30; descriptive, Frontline's reading.
        unsigned int scriptRotating : 1; // Bit 29; descriptive.
        unsigned int scriptScaling : 1; // Bit 28; set by ScaleFromScript.
        unsigned int hasMotion : 1;     // Bit 27; descriptive.
        unsigned int motionPlaying : 1; // Bit 26: cleared by StopMotionPlayback.
        unsigned int playReverse : 1;   // Bit 25; set by PlayAnimation on a backward range.
        unsigned int allowCollisionDamage : 1; // Bit 24.
        unsigned int pathFlag1 : 1;            // Bit 23: BIFunc_MechSetPathFlag selector 1.
        unsigned int dumpObject : 1;    // Bit 22; Destroy hands the object to DestroyDumpObject.
        unsigned int attached : 1;      // Bit 21; AttemptUpdate skips script motion. Descriptive.
        unsigned int unknown_20 : 1;
        unsigned int aimEnabled : 1;    // Bit 19; InitAim. Descriptive.
        unsigned int unknown_low : 19;
    };
    // The four sort-list nodes at +0x20; each keeps a bound as a float at +8. Names descriptive.
    struct SortNode {
        unsigned char unknown_0[8];
        float value;
    };
    unsigned char unknown_0[0x20];
    SortNode *sortMinX, *sortMaxX, *sortMinY, *sortMaxY; // +0x20
    unsigned char unknown_30[0x4c - 0x30];
    void *model;          // +0x4c; drawing is skipped while it is null.
    CMatrix transform;    // +0x50
    unsigned char unknown_90[0x100 - 0x90];
    float pathDistance;   // +0x100; descriptive, copied to +0x230 on commit.
    unsigned char unknown_104[0x110 - 0x104];
    float pathScale;      // +0x110; BIFunc_MechSetPathFlag stores value / 100.
    unsigned char unknown_114[0x12c - 0x114];
    unsigned int pathState; // +0x12c; cleared by Init. Descriptive.
    IVolume *localVolumeType3; // +0x130; returned for volume type 3.
    // +0x134: GetLocalBoundingVolume's type-0 volume, despite the older field name.
    IVolume *worldVolume; // +0x134; freed by CThrownObject::CleanUp, queried for GetRadius.
    IVolume *worldVolumeType3; // +0x138
    // +0x13c: GetWorldBoundingVolume's type-0 volume; typed int by earlier code.
    int field_13c;        // +0x13c; cleared by CThrownObject::CleanUp.
    BSObject *script;     // +0x140
    unsigned char unknown_144[0x150 - 0x144];
    CVector3 animPosition; // +0x150
    unsigned char unknown_160[0x180 - 0x160];
    CQuaternion animOrientation; // +0x180
    CVector3 velocity;    // +0x190; CThrownObject limits its speed.
    CVector3 pathPosition; // +0x1a0
    CQuaternion orientation; // +0x1b0; kept in step with the transform.
    unsigned char unknown_1c0[0x1e0 - 0x1c0];
    float scriptScaleTarget; // +0x1e0
    float scriptScaleBase; // +0x1e4; starts at 1 in Init.
    unsigned char unknown_1e8[0x1f0 - 0x1e8];
    float scriptScaleRate; // +0x1f0
    unsigned char unknown_1f4[0x1fc - 0x1f4];
    int scriptScaleEvent; // +0x1fc
    unsigned char unknown_200[0x20c - 0x200];
    int field_20c;        // +0x20c; the collision id. CThrownObject::LimitSpeed does nothing while -1.
    float aimValue;       // +0x210; InitAim. Descriptive.
    int maxLightPriority; // +0x214
    Flags flags;          // +0x218
    unsigned int mechFlags; // +0x21c; bits set and cleared by BIFunc_MechSetFlag.
    unsigned char unknown_220[0x230 - 0x220];
    float pathAngle;      // +0x230
    int motionEvent;      // +0x234; PlayAnimation's event.
    int motionFrame;      // +0x238
    int motionEnd;        // +0x23c
    int motionFrameCount; // +0x240
    unsigned char unknown_244[4];
    int motionSpeed;      // +0x248
    int motionStart;      // +0x24c
    unsigned char unknown_250[0x258 - 0x250];
    int motionKey;        // +0x258
    int motionLoop;       // +0x25c
    unsigned char unknown_260[4];
    float motionRate;     // +0x264
    int motionTimer;      // +0x268
    float motionSampleRate; // +0x26c
    float motionRemaining; // +0x270
    float motionStep;     // +0x274
    unsigned char unknown_278[0x290 - 0x278];
    MotionFrame *motionFrames; // +0x290
    unsigned char unknown_294[0x2a8 - 0x294];
    CLight *attachedLight; // +0x2a8
    unsigned char unknown_2ac[0x2c0 - 0x2ac];
    CLightVolumeManager lightVolumes; // +0x2c0
    CObjectFadeManager fade; // +0x2dc; extent unknown.
};

class CAnimObject {
  public:
    void Init();
    void CommitUpdate();
    void UpdateAnimation(float);
    unsigned char unknown_0[0x234];
    int allowAnimationTranslation; // +0x234; set by BIFunc_SetAllowAnimationTranslation.
    unsigned char unknown_238[0x294 - 0x238];
    int gravity;    // +0x294
    int groundCheck; // +0x298
    void SetAttached(bool);
    void StartAnimAdjust(const CVector3 &, float, bool);
};

class CSoldierObject {
  public:
    void Draw(CDrawContext &);
    void RemoveWeapons();
    // Original nested enum tag; members are descriptive.
    enum Partition { PartitionNone = 0, Partition1 = 1, Partition2 = 2, Partition3 = 3 };
    unsigned char unknown_0[0x120];
    BSObject *scriptObject; // +0x120
    unsigned char unknown_124[0x220 - 0x124];
    int groundCheckPending; // +0x220
    unsigned char unknown_224[4];
    int groundCheckState;   // +0x228; cleared before the ground-check event.
    unsigned char unknown_22c[0x2ec - 0x22c];
    int locomotionInfo;     // +0x2ec; returned by address.
    unsigned char unknown_2f0[0x334 - 0x2f0];
    int nonVisDestructDisabled; // +0x334; the inverse of BIFunc_EnableNonVisDestruct's flag.
    unsigned char unknown_338[0xb14 - 0x338];
    int canBePushed;        // +0xb14
    unsigned char unknown_b18[0xb84 - 0xb18];
    CWeapon *weapons[1];    // +0xb84; slot table, indexed by slot number (extent unknown).
    unsigned char unknown_b88[0xf20 - 0xb88];
    int currentWeaponSlot;  // +0xf20
    unsigned char unknown_f24[0xf7c - 0xf24];
    // Facial-expression blend state written by BIFunc_SetExpression; names are descriptive.
    struct ExpressionState {
        int current;        // +0xf7c
        int pending;        // +0xf80
        float currentBlend; // +0xf84
        float pendingBlend; // +0xf88
        float currentTime;  // +0xf8c
        float pendingTime;  // +0xf90
        float currentRate;  // +0xf94
        float pendingRate;  // +0xf98
    } expression;
    // Blink timing written by BIFunc_SetBlinkState; names are descriptive.
    float blinkTimer;       // +0xf9c
    float blinkMinInterval; // +0xfa0
    float blinkMaxInterval; // +0xfa4
    float blinkCloseTime;   // +0xfa8
    float blinkHoldTime;    // +0xfac
    float blinkOpenTime;    // +0xfb0
    unsigned char unknown_fb4[0x1050 - 0xfb4];
    int voiceHandle;        // +0x1050; -12 when no voice is playing.
    unsigned char unknown_1054[0x10a8 - 0x1054];
    CJawController jaw;     // +0x10a8
    unsigned char unknown_10e8[0x1148 - 0x10e8];
    int aimSuspended;       // +0x1148; set by BIFunc_SuspendAiming.
    unsigned char unknown_114c[0x12b4 - 0x114c];
    int propAimSuspended;   // +0x12b4; set by BIFunc_SuspendPropAiming.
    unsigned char unknown_12b8[0x15e8 - 0x12b8];
    int japanese;           // +0x15e8; set by BIFunc_SetJapanese.
    unsigned char unknown_15ec[0x1c54 - 0x15ec];
    float reloadTimer;      // +0x1c54; reset to -1 by BIFunc_BotDoneReloading.
    void SetHeadTrans(bool);
    CWeapon *GetCurrentWeapon() const;
    void SetUpperTranslation(const CVector3 &);
    void SetUpperXAngleDifference(float, float);
    void SetUpperZAngleDifference(float, float);
    void SetFaceAnimationState(unsigned short, int);
    void SetHeadTrackPlayer(bool);
    void EnableSkeletalPartition(Partition);
    void DisableSkeletalPartition(Partition);
    int SetAnimationState(const char *, int, Partition);
    ISceneNode *GetAttachedObjectAt(char *);
    void ResumeAimModifier();
    void ResumePropAimModifier();
    void EnablePropAimModifier(bool);
    void SaveGagState();
    void FireMMG(CStaticObject *);
    void SpecifyTurnAngle(float);
    void SetCurrentWeapon(int, int, bool);
    void EjectHelmet(CVector3 &);
    int IsInAnyOtherCompartment(int);
    void FireCurrentWeapon(float, EFireMethod);
};

// Method-only view of a hierarchy object.
// Inline in the original: the soldier's current weapon slot as a scene node.
inline ISceneNode *CurrentWeaponNode(CSoldierObject *soldier) { return (ISceneNode *)soldier->weapons[soldier->currentWeaponSlot]; }

class IVolume;
class CHierObject {
  public:
    IVolume *GetSubVolume(int) const;
    void RegisterBulletDamage(int, int, int, float);
    float GetBulletDamageMultiplierByClass(int, int);
    CStaticObject *GetSubObject(int);
    void ModifyMotionData(MotionData *);
};

class TriggerObject;
int AssetCRC(char *);
int *CreateRootHierObject(TriggerObject *, int, int, int);
void CreateSubHierObject(CHierObject *, int, int, unsigned int, int, int);
void CreateSubHierAnimObject(CHierObject *, int, int, unsigned int, int, int, int);
int *CreateObject(TriggerObject *, int, int, int, void *);

// Scoped view of a bounding volume; only the centre at +0x40 is read.
struct BoundingVolumeView {
    unsigned char unknown_0[0x40];
    CVector3 center;
};
// Static creators; the returned nodes are used through the scene-node tables.
class CProjector {
  public:
    static IMovingSceneNode *Create(unsigned int, bool);
};
class CGlare {
  public:
    static IMovingSceneNode *Create(int, bool);
};

// Static animated-texture operations keyed by texture CRC.
class AnimShape {
  public:
    // Original nested enum tag; the observed values are ASCII state letters.
    enum EState { StateDefault = '-', StateL = 'L', Statel = 'l', StateO = 'O', Stateo = 'o', StateP = 'P', Statep = 'p' };
    static void SetStateByCRC(int, EState);
    static void InitByCRC(int, EState, float);
    static void ResetByCRC(int);
    static void AdvanceByCRC(int);
    static void SetFrameByCRC(int, float);
    static void SetFrameRateByCRC(int, float);
};

// Static decal operations used by the script built-ins.
class CDecalManager {
  public:
    static void ClearDecals();
};

// Four-byte colour passed by value; channel names are descriptive.
class CColor {
  public:
    unsigned char r, g, b, a;
    CColor() {}
    CColor(unsigned char ar, unsigned char ag, unsigned char ab, unsigned char aa) : r(ar), g(ag), b(ab), a(aa) {}
};

// Static screen-fade entry points.
class CFader {
  public:
    static void Flash(CColor, float, float);
    static void FadeIn(CColor, float);
    static void FadeOut(CColor, float);
};

// Static motion-blur control.
class MotionBlur {
  public:
    static void SetFrameBlur(float, float, float, float);
};

// Virtual-interface view of the animated-object tables (slots 89-113 after the
// IMovingSceneNode slots), in the order of the original CSoldierObject table.
// Names come from the symbols in each slot; return and parameter types beyond
// the symbols are placeholders. It declares no storage: callers convert the
// scene-node pointers they already hold, which point at the same object.
namespace EALA { class Task; }
namespace EAGL { class Transform; }
enum EAnimCallback { AnimCallbackUnknown = 0 };
class CAnimObjectInterface : public IMovingSceneNode {
  public:
    virtual void Init();
    virtual void Shutdown();
    virtual void CleanUp();
    virtual int SetAnimationState(const char *, int);
    virtual int SetAnimationState(const char *, EALA::Task *);
    virtual void UpdateAnimation(float);
    virtual void SetMoveTarget(const CVector3 &);
    virtual void SetScript(BSObject *);
    virtual void UpdateLocalBoundingVolume(ISceneNode::EVolumeType, const CVector3 &, const CVector3 &);
    virtual int LineAlmostCollided(CBullet *) const;
    virtual void GetTMLocalToWorld(EAGL::Transform &) const;
    virtual int AttachObject(CStaticObject *, char *, int);
    virtual int DetachObject(char *);
    virtual int GetAttachPointMatrix(char *, CMatrix *);
    virtual void OnSwordDraw();
    virtual void OnSwordSheath();
    virtual void AttachFloatingBone();
    virtual void DetachFloatingBone();
    virtual void SetAnimCallback(EAnimCallback);
    virtual float FadeValue() const;
    virtual void FadeOut(float);
    virtual void FadeIn(float);
    virtual int AttachObject(int, char *, int);
    virtual void GetAimTarget(CVector3 &) const;
    virtual void EnableAimModifier(bool);
};

// Virtual-interface view of the static/hierarchy-object tables (slots 89-109
// after the IMovingSceneNode slots), in the order of the original CHierObject
// table. Names come from the symbols in each slot; types beyond the symbols are
// placeholders. Callers convert the scene-node pointers they already hold.
class CPlayerObject;
class CAnimObject;
class CDrawContext;
class CAttachableObject {
  public:
    unsigned char unknown_0[0x4c];
    // The static mesh record is reached through a pointer at +0x4c; +0x30 holds the
    // attach-node count and +0x50 the first 80-byte node, with its id at +0x40.
    struct AttachMesh {
        struct Node {
            CMatrix matrix;
            int id;
            unsigned char unknown_44[12];
        };
        unsigned char unknown_0[0x30];
        int nodeCount;
        unsigned char unknown_34[0x50 - 0x34];
        Node nodes[1];
    } **mesh;
    unsigned char unknown_50[0x300 - 0x50];
    ISceneNode *children[20]; // +0x300
    int GetFreeAttachSlot();
    void Draw(CDrawContext &);
    int IsVisible(CDrawContext &) const;
    void BeginUpdate(float);
    void UpdateAI(float);
    void CommitAI();
    void AttemptUpdate(float);
    void CommitUpdate();
    int FindAttachNode(int, CMatrix *);
    int GetObjectType() const;
    void UpdateAttachedObjects();
    int AttachObject(CPlayerObject *, int, bool);
    int AttachObject(CAnimObject *, int);
    int AttachObject(CStaticObject *, int);
    int DetachObject(ISceneNode *);
    void DetachAllObjects();
    int AttachAndAlignObject(CAnimObject *, int);
    int GetFirstChildHandle();
    int GetNextChildHandle(int);
    ISceneNode *GetChild(int);
};
class CTankObject {
  public:
    unsigned char unknown_0[0xb50];
    int lookObject; // +0xb50; set by BIFunc_HierObjectSetLookObject.
    void FireSubObject(int);
    void MapWeaponSlot(int, int);
};
class CStaticObjectInterface : public IMovingSceneNode {
  public:
    virtual void Init();
    virtual void SetScript(BSObject *);
    virtual int GetObjectType() const;
    virtual void TranslateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum);
    virtual void RotateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum);
    virtual void ScaleFromScript(float, float, EBSEventEnum);
    virtual void StartMotionPlayback(int, int, EBSEventEnum);
    virtual void *AsThrownObject();
    virtual void *AsWeaponObject();
    virtual CTankObject *AsHierTankObject();
    virtual void AttemptScriptMotionUpdate(float);
    virtual void AttemptAnimMotionUpdate(float);
    virtual void AttemptPathMotionUpdate(float);
    virtual void CommitScriptMotionUpdate();
    virtual void CommitAnimMotionUpdate();
    virtual void CommitPathMotionUpdate();
    virtual int FindAttachNode(int, CMatrix *);
    virtual void CleanUp();
    virtual int SetAnimationState(const char *, int);
    virtual void BeginScriptMotionUpdate(float);
    virtual void BeginAnimMotionUpdate(float);
};

#endif
