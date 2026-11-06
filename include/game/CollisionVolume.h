// AI-assisted scoped reconstruction from GR8E69; see docs/Collision.md. The
// header name is a project choice.
#ifndef GAME_COLLISIONVOLUME_H
#define GAME_COLLISIONVOLUME_H
#include "CMatrix.h"
#include "SceneNode.h"
#include "Volume.h"

class IVolume;
struct BSObject;

struct ExtentEntry {
    int unknown_00[2];
    float value;
};

// The matrix at +0x40, volume at +0x80, id at +0x84 and script at +0x88 are
// established by the methods; the words at +0x14 to +0x34 are cleared by the
// constructor. Names of the fields are descriptive.
class CCollisionVolume : public ISceneNode {
  public:
    CCollisionVolume();
    virtual ~CCollisionVolume();
    virtual void MarkForDestruction(int);
    virtual void Destroy();
    virtual void BeginUpdate(float);
    void Init(IVolume *, CMatrix *, EClsnId);
    virtual void GetTMLocalToWorld(CMatrix &) const;
    virtual void GetPosition(CVector3 &) const;
    virtual void GetRightward(CVector3 &) const;
    virtual void GetForward(CVector3 &) const;
    virtual void GetUpward(CVector3 &) const;
    virtual int GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    virtual int GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    virtual EClsnId GetCollisionId() const;
    virtual void SetCollisionId(EClsnId);
    virtual int GetScriptObject() const;
    virtual CCollisionVolume *AsCollisionVolume();
    virtual const CCollisionVolume *AsCollisionVolume() const;
    virtual void SetScript(BSObject *);
    virtual void HandleBulletCollision(CBullet *, const CCollision &);

    int unknown_18[2];
    ExtentEntry *extentLowX;
    ExtentEntry *extentHighX;
    ExtentEntry *extentLowY;
    ExtentEntry *extentHighY;
    int unknown_30[2];
    int unknown_38[2];
    CMatrix localToWorld;
    IVolume *volume;
    EClsnId collisionId;
    BSObject *script;
};

#endif
