// AI-assisted scoped reconstruction from GR8E69; see docs/Bullets.md.
#ifndef GAME_THROWN_H
#define GAME_THROWN_H
#include "CVector3.h"
#include "SceneObjects.h"
#include "Volume.h"
// The properties chain behind CThrownObject +0x350.
struct ThrownProperties {
    unsigned char unknown_0[8];
    float value;
};
struct ThrownDefinitionData {
    unsigned char unknown_0[60];
    ThrownProperties *friction;     // +60
    ThrownProperties *restitution;  // +64
    ThrownProperties *minRollSpeed; // +68; Draw's speed threshold.
    unsigned char unknown_48[84 - 72];
    ThrownProperties *impulseScale; // +84; HandleBulletCollision's scale.
};
struct ThrownDefinition {
    unsigned char unknown_0[8];
    ThrownDefinitionData *data;
};
// Scoped view: the direct call to CStaticObject::MarkForDestruction establishes the
// base. Bases of CStaticObject and the virtual interface are not declared.
class CThrownObject : public CStaticObject {
  public:
    unsigned char unknown_220[0x300 - sizeof(CStaticObject)];
    float lifetime;        // +0x300; name from Frontline.
    unsigned char unknown_304[0x310 - 0x304];
    CVector3 lastVelocity; // +0x310
    CVector3 field_320;    // +0x320; cleared by AttemptUpdate.
    CVector3 stepVelocity; // +0x330; velocity times the time step.
    CQuaternion rollOrientation; // +0x340; turned by Draw. Name inferred.
    ThrownDefinition *definition; // +0x350; only the chain used by Draw and HandleBulletCollision is known.
    int pendingSound;      // +0x354; the low halfword is the impact sound. Name from Frontline.
    unsigned char scriptNode[0x394 - 0x358]; // +0x358; a BSGO_Basic.
    union {
        unsigned int thrownFlags; // +0x394
        struct {
            unsigned int inContact : 1;   // Bit 31; names inferred from Frontline.
            unsigned int touched : 1;     // Bit 30.
            unsigned int touched2 : 1;    // Bit 29.
            unsigned int unknown_28 : 1;
            unsigned int deleted : 1;     // Bit 27.
            unsigned int marked : 1;      // Bit 26; set once marked for destruction.
            unsigned int unknown_low : 26;
        } bits;
    };
    int state;             // +0x398; -1 is thrown, 2 is landed (Frontline's reading).
    int impactSound;       // +0x39c
    int drawArgument;      // +0x3a0; copied to the draw context around the mesh draw.
    int Constrain(float);
    void OnCollision(const CCollision &);
    void HandleBulletCollision(CBullet *, const CCollision &);
    void Draw(CDrawContext &);
    void BeginUpdate(float);
    void AttemptUpdate(float);
    void UpdateCollisionVolumes();
    void CommitUpdate();
    void CleanUp();
    void SetVelocity(CVector3 &);
    void AddVelocity(CVector3 &);
    void LimitSpeed();
    void MarkForDestruction(int);
    void Destroy();
    void IGO_Recycle();
    int GetObjectType() const;
    CThrownObject *AsThrownObject();
    void TranslateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum);
    void RotateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum);
    void ScaleFromScript(float, float, EBSEventEnum);
};
#endif
