#ifndef GAME_OBJ_CREATE_H
#define GAME_OBJ_CREATE_H
// State of the original objcreate.cpp. Global, class and struct names come from the symbol table;
// member names, the two queue entry structs and this header's name are descriptive.
#include "BSGameObject.h"
#include "../game-containers/TargetTypes.h"
class BSObject;
class CStaticObject;
class ISceneNode;

class PickupInfo {
  public:
    int value00;      // +0x00
    int value04;      // +0x04
    int value08;      // +0x08
    int value0c;      // +0x0c
    int value10;      // +0x10; not cleared by HitDumpObject
    int value14;      // +0x14
    int value18;      // +0x18; not cleared by HitDumpObject
    BSObject *script; // +0x1c
    int value20;      // +0x20
};
struct DumpObject {
    CStaticObject *object;
    PickupInfo pickup; // +0x04
    int keep;          // +0x28
    int inUse;         // +0x2c
    DumpObject *next;  // +0x30
    DumpObject *previous; // +0x34
};
class CDumpObjectContainer {
  public:
    DumpObject *objects; // +0x00
    DumpObject *first;   // +0x04
    DumpObject *last;    // +0x08
    int count;           // +0x0c
    int capacity;        // +0x10
    CDumpObjectContainer();
    ~CDumpObjectContainer();
    void Reset();
    void AddDumpObject(CStaticObject *, const PickupInfo &, bool);
    void DeleteDumpObject(DumpObject *);
    DumpObject *GetDumpObject(CStaticObject *);
    DumpObject *AllocateDumpObject();
    void FreeDumpObject(DumpObject *);
    DumpObject *GetAvailableObject();
    DumpObject *DeleteOldest(bool);
    DumpObject *GetFirstDumpObject();
    DumpObject *GetNextDumpObject(DumpObject *);
};

extern CDumpObjectContainer g_DumpObjectContainer;
void MarkDumpObjectForDestruction(DumpObject *);

class TriggerObject;
class BSUtilObjectInstanceMemoryAllocator;
struct DelayDestroyEntry {
    BSGO_Basic *object; // +0x00
    int frame;     // +0x04; g_frameCounter when queued
};
struct GermanCreationEntry {
    TriggerObject *trigger; // +0x00
    int index;              // +0x04
    int value08;            // +0x08
    int wait;               // +0x0c; frames spent in the queue
};
extern BSUtilObjectInstanceMemoryAllocator g_pObjectAllocators[2];
extern BSUtilObjectInstanceMemoryAllocator g_pVolumeAllocator;
extern BSUtilObjectInstanceMemoryAllocator g_pAnimatedVolumeAllocator;
extern BSUtilObjectInstanceMemoryAllocator g_pEnvModMechAllocator;
extern GermanCreationEntry g_pGermanCreationQueue[15];
extern DelayDestroyEntry g_pDelayDestroyList[10];
extern int g_delayDestroyHead;
extern int g_delayDestroyTail;
extern unsigned int g_frameCounter;
extern void *g_pBoxList;
extern void *g_pGermanList;
extern void *g_pTankList;
extern void *g_pAnimatedMechList;
extern int g_numActiveSoldiers;
extern int g_maxActiveSoldiers;
extern float g_fDumpObjectCollisionRadius;
extern int g_iNumberOfEnemiesSpawned;
extern int g_iGermanCreationQueueHeadIndex;
extern int g_iGermanCreationQueueTailIndex;
extern int g_iNumGermansQueuedThisFrame;

// Each object reaches its scene node through this block; it sits after the script data, whose size
// differs per class.
struct BSGOBindingLink {
    BSGOBindingLink *next;     // +0x00
    BSGOBindingLink *previous; // +0x04
};
struct BSGOBinding {
    unsigned char unknown00[8];
    BSGOBindingLink link; // +0x08
    unsigned char unknown10[4];
    ISceneNode *sceneNode; // +0x14
};

// A stand-in game object that lets the proximity trigger code see a scene node with no script.
class BSGO_Dummy : public BSGO_Basic {
  public:
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    WeakPtr<ISceneNode, 8> sceneNode; // +0x14
    int proximityData[4];             // +0x2c
};
class BSGO_Destructable : public BSGO_Basic {
  public:
    virtual ~BSGO_Destructable();
    void Destroy();
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    static void *operator new(unsigned int);
    static void operator delete(void *);
    unsigned char scriptData[0x20 - 0x14];
    BSGOBinding binding;              // +0x20
    unsigned char proximityData[4];   // +0x38; extent unknown.
};
class CAnimObject;
class BSGO_Anim : public BSGO_Basic {
  public:
    virtual ~BSGO_Anim();
    void Destroy();
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    static void *operator new(unsigned int);
    static void operator delete(void *);
    void *scriptData;               // +0x14; filled by CreateBSObject.
    WeakPtr<CAnimObject, 8> target; // +0x18
    int proximityData[4];           // +0x30
};
class CSoldierObject;
class BSGO_Soldier : public BSGO_Basic {
  public:
    virtual ~BSGO_Soldier();
    void Destroy();
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    static void *operator new(unsigned int);
    static void operator delete(void *);
    // Script data view: CreateGerman fills these; the fields' meanings are unknown except the first.
    struct {
        float hitPoints;   // +0x14
        void *scriptState; // +0x18; filled by CreateBSObject.
        unsigned char unknown_1c[0x3c - 0x1c];
        int value3c;       // +0x3c
        int value40;       // +0x40
        int value44;       // +0x44
        int value48;       // +0x48
        int value4c;       // +0x4c
        int value50;       // +0x50
        unsigned char unknown_54[0xe4 - 0x54];
    } scriptData;
    WeakPtr<CSoldierObject, 8> target; // +0xe4
    int proximityData[4];              // +0xfc
};
class BSGO_Tank : public BSGO_Basic {
  public:
    virtual ~BSGO_Tank();
    void Destroy();
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    static void *operator new(unsigned int);
    static void operator delete(void *);
    unsigned char scriptData[0xe4 - 0x14];
    BSGOBinding binding;              // +0xe4
    unsigned char proximityData[4];   // +0xfc; extent unknown.
};
class BSGO_Player : public BSGO_Basic {
  public:
    virtual ~BSGO_Player();
    void Destroy();
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
    unsigned char scriptData[0x38 - 0x14];
    ISceneNode *sceneNode;          // +0x38
    unsigned char proximityData[4]; // +0x3c; extent unknown.
};

#endif
