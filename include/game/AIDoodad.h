// AI-assisted scoped reconstruction from GR8E69, with Medal of Honor: Frontline's
// doodads as a semantic reference. Class and method names are original symbols;
// member names are descriptive. Filter classes declare only what the doodads call.
#ifndef GAME_AI_DOODAD_H
#define GAME_AI_DOODAD_H
#include "AIBehavior.h"
extern "C" void *DWI_alloc(const char *, int, int);
struct BSObject;
class CPlayerObject;
class CBotObject;
class CSoldierObject;
class CTankObject;
class CAIPlayerObject {
public:
    void Init();
};
class CAIFilterPlayerObject : public CAIFilterObject {
public:
    CAIFilterPlayerObject(BSObject *);
    void SetOneTimeVars();
    void UpdateManualUpdateVars();
    CAIPlayerObject *GetAIObject() const;
    static void *operator new(unsigned int size, const char *label) {
        return DWI_alloc(label, size, 1024);
    }
};
class CAIFilterSoldierObject : public CAIFilterObject {
public:
    CAIFilterSoldierObject(BSObject *);
    void *operator new(unsigned int, void *);
    void SetOneTimeVars();
    void UpdateManualUpdateVars();
    CAISoldierObject *GetAIObject() const;
};
class CAIFilterTankObject : public CAIFilterObject {
public:
    CAIFilterTankObject(BSObject *);
    void *operator new(unsigned int, void *);
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
