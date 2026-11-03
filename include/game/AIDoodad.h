// AI-assisted scoped reconstruction from GR8E69, with Medal of Honor: Frontline's
// doodads as a semantic reference. Class and method names are original symbols;
// member names are descriptive. Filter classes declare only what the doodads call.
#ifndef GAME_AI_DOODAD_H
#define GAME_AI_DOODAD_H
#include "AIBehavior.h"
extern "C" void *DWI_alloc(const char *, int, int);
#include "SceneNode.h"
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
struct BSObject;
class CPlayerObject;
class CBotObject;
class CSoldierObject;
class CTankObject;
class CAISoldierDoodad;
// The player's AI object: table pointer at +0x72c through CAIObjectInterface. The
// original base is CAIObject; its constructor and destructor are not modelled here.
class CAIPlayerObject : public CAIObjectInterface {
public:
    CAIPlayerObject(void *);
    virtual ~CAIPlayerObject();
    void Init();
    void SetInitialStatus();
    void UpdateStatus();
    virtual void *AsAIPlayerObject();
    virtual void PreUpdate();
    virtual void Update(float);
    virtual void UpdateFromGameObject(IMovingSceneNode *);
    virtual bool IsCrouching() const;
    virtual int IsPlayer() const;
    virtual void GetLookDirection(CVector3 &, CVector3 &);
    virtual ISceneNode *GetSceneNode();
    float health;        // +0x730
    float maxHealth;     // +0x734
    ISceneNode *sceneNode; // +0x738
    float nextFieldOfViewCheck; // +0x73c
};
class CAIFilterPlayerObject : public CAIFilterObject {
public:
    CAIFilterPlayerObject(BSObject *);
    virtual ~CAIFilterPlayerObject();
    void SetOneTimeVars();
    void UpdateManualUpdateVars();
    CAIPlayerObject *GetAIObject() const;
};
// The AI object is placement-constructed in the aligned storage that follows the filter.
class CAIFilterSoldierObject : public CAIFilterObject {
public:
    CAIFilterSoldierObject(BSObject *);
    virtual ~CAIFilterSoldierObject();
    void *operator new(unsigned int, void *);
    void operator delete(void *);
    void SetOneTimeVars();
    void UpdateManualUpdateVars();
    virtual void SetMoveTarget(const CVector3 &);
    CAISoldierDoodad *GetAIDoodad() const;
    CAISoldierObject *GetAIObject() const;
};
class CAIFilterTankObject : public CAIFilterObject {
public:
    CAIFilterTankObject(BSObject *);
    virtual ~CAIFilterTankObject();
    void *operator new(unsigned int, void *);
    void operator delete(void *);
    void SetOneTimeVars();
    void UpdateManualUpdateVars();
    CAITankObject *GetAIObject() const;
};
class CAIPlayerDoodad : public CAIDoodad {
public:
    CAIPlayerDoodad();
    virtual ~CAIPlayerDoodad();
    virtual ISceneNode *GetSceneNode() const;
    virtual CAIObject *GetAIObject() const;
    void Init(CPlayerObject *);
    void CleanUp();
    CPlayerObject *player; // +0x08
};
class CAISoldierDoodad : public CAIDoodad {
public:
    CAISoldierDoodad();
    virtual ~CAISoldierDoodad();
    virtual ISceneNode *GetSceneNode() const;
    virtual CAIObject *GetAIObject() const;
    void Init(CBotObject *);
    void Init(CSoldierObject *);
    void Init();
    void CleanUp();
    void SetUpperTranslation(const CVector3 &);
    void SetUpperXAngleDifference(float, float);
    void SetUpperZAngleDifference(float, float);
    CSoldierObject *soldier; // +0x08; also holds a CBotObject.
};
class CAITankDoodad : public CAIDoodad {
public:
    CAITankDoodad();
    virtual ~CAITankDoodad();
    virtual ISceneNode *GetSceneNode() const;
    virtual CAIObject *GetAIObject() const;
    void Init(CTankObject *);
    void CleanUp();
    CTankObject *tank; // +0x08
};
#endif
