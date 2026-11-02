// AI-assisted scoped reconstruction from GR8E69; see docs/Bullets.md.
#ifndef GAME_THROWN_H
#define GAME_THROWN_H
#include "CVector3.h"
#include "SceneObjects.h"
#include "Volume.h"
// Scoped view: the direct call to CStaticObject::MarkForDestruction establishes the
// base. Bases of CStaticObject and the virtual interface are not declared.
class CThrownObject : public CStaticObject {
  public:
    unsigned char unknown_220[0x310 - sizeof(CStaticObject)];
    CVector3 lastVelocity; // +0x310
    unsigned char unknown_320[0x394 - 0x320];
    unsigned int thrownFlags; // +0x394; bit 26 is set once marked for destruction.
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
