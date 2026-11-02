// AI-assisted scoped reconstruction from GR8E69; see docs/Bullets.md.
#ifndef GAME_BULLET_H
#define GAME_BULLET_H
#pragma interface
#include "CMatrix.h"
#include "SceneNode.h"

#include "ParticleRecipe.h"
#include "LightVolumeManager.h"
#include "Volume.h"
#include "Sphere.h"
class CCollision;
class CProjectileBullet;
class CThrownBullet;

// Original enum tag; script bullet-type numbers are used directly.
enum EScriptBulletType { ScriptBulletTypeUnknown = 0 };
int GetClassFromBulletType(EScriptBulletType);

// A collision-sort axis entry; only its value at +8 is written here.
struct BulletExtentEntry {
    unsigned char unknown_0[8];
    float value;
};

// CBullet's table (0x802e9eb0) follows the IMovingSceneNode slot map and adds slots
// 89-102 in this order. Field names are descriptive; unlisted storage is unknown.
class CBullet : public IMovingSceneNode {
  public:
    enum EXPLOSION_PARTICLE_TYPE { ExplosionParticleTypeUnknown = 0 };

    int field_18, field_1c;  // Cleared by Init unless its argument is set.
    BulletExtentEntry *extentMinX, *extentMaxX, *extentMinY, *extentMaxY; // +0x20; set from the fiber's extents. Names from Frontline.
    unsigned char unknown_30[0x40 - 0x30];
    EClsnId collisionId;
    int bulletKind; // +0x44; 1 for projectile bullets, tested by CBulletFactory::DestroyBullet.
    unsigned char unknown_48[0x50 - 0x48];
    CVector3 position;     // +0x50
    CVector3 prevPosition; // +0x60
    CMatrix localToWorld;
    float field_b0, field_b4;

    void Draw(CDrawContext &);
    unsigned int IsDrawEnabled() const;
    void GetPosition(CVector3 &) const;
    void GetRightward(CVector3 &) const;
    void GetForward(CVector3 &) const;
    void GetTMLocalToWorld(CMatrix &) const;
    CBullet *AsBullet();
    const CBullet *AsBullet() const;

    virtual void Init(bool);
    virtual void Shutdown();
    virtual void GetUp(CVector3 &) const;
    virtual float GetDamage() const;
    virtual int GetScriptBulletType() const;
    virtual ISceneNode *GetFiredBy() const;
    virtual CThrownBullet *AsThrown();
    virtual CProjectileBullet *AsProjectile();
    virtual void GetVelocity(CVector3 &);
    virtual void SetExclusionPair(ISceneNode *);
    virtual void Penetrate(const CCollision &);
    virtual void SetDamage(float);
    virtual void SetBlastRadius(float);
    virtual void SetExplosionParticleSystem(unsigned long, EXPLOSION_PARTICLE_TYPE);
};

// Scoped record views: only the fields the accessors read are named.
struct ProjectileBulletProperties_struct {
    unsigned int crc;      // +0x00; searched by CBulletFactory::GetBulletByCRC.
    float weight;          // +0x04; returned by CBulletFactory::GetBulletWeight.
    unsigned char unknown_08[72 - 8];
    void *sprite;          // +0x48
    float damage;          // +0x4c
    unsigned char unknown_50[84 - 80];
    float width;           // +0x54; Draw does nothing while it is zero. Name from Frontline.
    unsigned char unknown_58[96 - 88]; // The factory indexes the table in 96-byte steps.
};
struct ThrownBulletProperties_struct {
    unsigned int crc;      // +0x00
    float weight;          // +0x04
    unsigned char unknown_08[72 - 8];
    void *sprite;          // +0x48
    unsigned char unknown_4c[84 - 76];
    float friction;        // +0x54; names from Frontline.
    float bounce;          // +0x58
    float damage;          // +0x5c
    float blastRadius;     // +0x60
    unsigned char unknown_64[2];
    unsigned char timed;   // +0x66; SetDetonationTime is ignored unless set.
    bool IsTimed() const { return timed != 0; }
    unsigned char unknown_67; // The factory indexes the table in 104-byte steps.
};

// Bit 28 (counting from the least significant bit) of the word at +0x10c.
struct ProjectileBulletFlags {
    unsigned int hitPending : 1; // Bit 31: set around HandleBulletCollision; names descriptive.
    unsigned int hit : 1;        // Bit 30
    unsigned int shutDown : 1;   // Bit 29
    unsigned int drawForward : 1;
    unsigned int unknown_low : 28;
};

class CProjectileBullet : public CBullet {
  public:
    unsigned char unknown_b8[0xc4 - 0xb8];
    int type;                                       // +0xc4
    ProjectileBulletProperties_struct *properties; // Stored by Init.
    unsigned char unknown_cc[0xe0 - 0xcc];
    ISceneNode *firedBy;                            // Returned by GetFiredBy.
    unsigned char unknown_e4[0xfc - 0xe4];
    float speed;                                    // +0xfc; scales the direction in GetVelocity.
    unsigned char unknown_100[0x104 - 0x100];
    float life;                                     // +0x104
    float collisionTime;                            // +0x108; taken from a collision.
    ProjectileBulletFlags flags;
    unsigned char localVolume[0x160 - 0x110];       // +0x110
    unsigned char worldVolume[0x1b0 - 0x160];       // +0x160; a CVolFiber (extent inferred).
    IParticleSystem *trail;                         // +0x1b0; deactivated by MarkForDestruction.

    void BeginUpdate(float);
    void AttemptUpdate(float);
    void CommitUpdate();
    void Penetrate(const CCollision &);
    void Draw(CDrawContext &);
    static void ExecuteDraw(CDrawContext &, void **);
    int GetScriptBulletType() const;
    void MarkForDestruction(int);
    void Destroy();
    void Shutdown();
    void OnCollision(const CCollision &);
    int IsVisible(CDrawContext &) const;
    void GetVelocity(CVector3 &);
    int GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    int GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    EClsnId GetCollisionId() const;
    float GetDamage() const;
    ISceneNode *GetFiredBy() const;
    CProjectileBullet *AsProjectile();
    void SetExclusionPair(ISceneNode *);
    void SetDamage(float);
    virtual void SetDrawForward(bool);
};

class CThrownBullet : public CBullet {
  public:
    void SomebodyCaughtYou();
    void AttemptUpdate(float);
    int Constrain(float);
    void BeginUpdate(float);
    void OnCollision(const CCollision &);
    void Penetrate(const CCollision &);
    unsigned char unknown_b8[0xc4 - 0xb8];
    int type;                                       // +0xc4
    ThrownBulletProperties_struct *properties;     // Stored by Init.
    unsigned char unknown_cc[0xe0 - 0xcc];
    ISceneNode *firedBy;                            // Returned by GetFiredBy.
    unsigned char scriptNode[0x120 - 0xe4];         // +0xe4; a BSGO_Basic.
    CLightVolumeManager lightVolumes;               // +0x120
    unsigned char unknown_13c[0x140 - 0x13c];
    CVector3 velocity;                              // +0x140
    CVector3 spin;                                  // +0x150; cleared with the velocity by Halt.
    CVector3 field_160;                             // +0x160; Penetrate's surface normal.
    CVector3 field_170, field_180, field_190;       // +0x170: field_190 times the distance moved, plus field_180.
    unsigned char localVolume[0x1f0 - 0x1a0];       // +0x1a0
    unsigned char worldVolume[0x254 - 0x1f0];       // +0x1f0; a CVolFiber (extent inferred).
    CLight *attachedLight;
    unsigned char unknown_258[0x25c - 0x258];
    unsigned int bit31 : 1;
    unsigned int bit30 : 1;
    unsigned int bit29 : 1;       // Bit 29; AttemptUpdate applies gravity while set.
    unsigned int explodeNow : 1;  // Bit 28; Explode sets it to the inverse of bit 27.
    unsigned int caught : 1;      // Bit 27; set by SomebodyCaughtYou.
    unsigned int unknown_26_25 : 2;
    unsigned int noCatch : 1;     // Bit 24; SomebodyCaughtYou does nothing while set.
    unsigned int unknown_low : 24;
    unsigned char unknown_260[0x264 - 0x260];
    // Four observed slots at +0x264..+0x270, initialized by CThrownBullet::Init.
    // This remains a scoped view, not a complete object allocation.
    unsigned long explosionParticleSystems[4];

    EClsnId GetCollisionId() const;
    float GetDamage() const;
    ISceneNode *GetFiredBy() const;
    CThrownBullet *AsThrown();
    void SetDamage(float);
    void SetBlastRadius(float);
    void SetExplosionParticleSystem(unsigned long, EXPLOSION_PARTICLE_TYPE);
    CLight *GetAttachedLight() const;
    void SetDetonationTime(float);
    void Explode();
    void HandleBulletCollision(CBullet *, const CCollision &);
    void Halt();
    void ApplyForceTo(const CVector3 &, float);
    void EnterLightVolume(BPDLightVolume *);
    void ExitLightVolume(BPDLightVolume *);
    BPDLightVolume *GetLightVolume();
    int GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    int GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    void SetLightVolumeTransitionDuration(float);
    void GetVelocity(CVector3 &);
    int IsVisible(CDrawContext &) const;
    void Destroy();
    void Shutdown();
    virtual unsigned int IsCollisionEnabled() const;
};

// Scoped view of the hit record passed to the damage built-in.
struct ScriptHitInfo {
    int unknown_0;
    float damage;                  // +0x4
    int unknown_8;
    EScriptBulletType bulletType;  // +0xc
    int location;                  // +0x10
};
class CScene;
class ISceneNode;
class CVector3;
// Stack-constructed blast; its constructor writes through +0x44. The script
// built-ins fix its frame at 0x4c or 0x50 bytes; 0x4c is used here.
class CExplosion;
class CExplosion : public dwi::IVisitor<ISceneNode> {
  public:
    unsigned char unknown_4[0x10 - 4];
    CVolSphere sphere;           // +0x10
    float strength;              // +0x30; scales the noise. Name inferred.
    float damage;                // +0x34
    EScriptBulletType bulletType; // +0x38
    ISceneNode *source;          // +0x3c
    bool heightCheck;            // +0x40; name inferred.
    bool field_44;               // +0x44
    unsigned char unknown_48[4];
    CExplosion(CScene &, const CVector3 &, float, float, float, EScriptBulletType, ISceneNode *, bool, bool);
    virtual ~CExplosion();
    virtual bool Visit(ISceneNode &);
    bool TestRay(const CVector3 &, const CVector3 &, const CVector3 &, const CMatrix &);
    bool CheckForObstructions(ISceneNode &);
    bool ShouldHeightCheckBeFlipped(ISceneNode &);
};
#endif
