#ifndef GAME_OBJ_CREATE_H
#define GAME_OBJ_CREATE_H
// State of the original objcreate.cpp. Global, class and struct names come from the symbol table;
// member names, the two queue entry structs and this header's name are descriptive.
class BSObject;
class CStaticObject;

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
    void *object;  // +0x00
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

#endif
