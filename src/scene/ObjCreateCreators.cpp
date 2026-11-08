// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp. Allocator and queue names come from the symbol
// table. The structures reached from TLT resources are not reconstructed: only the chains to the
// values read here are established.
#include "PlacementNew.h"
#include "ScriptPool.h"
#include "SceneObjects.h"
#include "ScriptEvents.h"
#include "ScriptTimers.h"
#include "ParticleClock.h"
#include "TriggerObject.h"
#include "ObjCreate.h"
#include <string.h>

struct LevelFileContentsStruct_;
enum TLTResourceID { TLTResourceUnknown = 0 };
struct TLTResource;
TLTResource *TLT_FindNextResourceByType(TLTResourceID, LevelFileContentsStruct_ *);

void DestroySingleQueueElement();
void ProcessCreationQueue();
void NonVisDestructFarthestSoldiers(int);
BSGO_Soldier *CreateGerman(TriggerObject *, int, int, bool, bool *);
void *CreateMechanic(TriggerObject *, int, int, void *);
void *CreateBasicAnimatedMech(TriggerObject *, int, int);
void *CreateEnvModMechanic(TriggerObject *, int);
void *CreateEnemy(TriggerObject *, int, int);
void *CreatePowerup(TriggerObject *, int, int);
void *CreateBots(TriggerObject *);
void BotSetup();

int CreateObjectGetMemoryRequirements() {
    int size = g_pObjectAllocators[0].ComputeNeededSize(72, 200, 0);
    size += g_pObjectAllocators[1].ComputeNeededSize(268, 25, 0);
    size += (16 - (size & 15)) & 15;
    size += g_pVolumeAllocator.ComputeNeededSize(80, 450, 16);
    size += (16 - (size & 15)) & 15;
    size += g_pEnvModMechAllocator.ComputeNeededSize(144, 50, 16);
    size += (16 - (size & 15)) & 15;
    return size + g_pAnimatedVolumeAllocator.ComputeNeededSize(64, 50, 16);
}

void InitObjectCreators() {
    int size = CreateObjectGetMemoryRequirements();
    char *memory = (char *)BSUtilGetMemory(size, 16);
    g_pObjectAllocators[0].Init(memory, 72, 200, 0);
    memory += g_pObjectAllocators[0].ComputeNeededSize(72, 200, 0);
    g_pObjectAllocators[1].Init(memory, 268, 25, 0);
    memory += g_pObjectAllocators[1].ComputeNeededSize(268, 25, 0);
    memory += (16 - ((int)memory & 15)) & 15;
    g_pVolumeAllocator.Init(memory, 80, 450, 16);
    memory += g_pVolumeAllocator.ComputeNeededSize(80, 450, 16);
    memory += (16 - ((int)memory & 15)) & 15;
    g_pEnvModMechAllocator.Init(memory, 144, 50, 16);
    memory += g_pEnvModMechAllocator.ComputeNeededSize(144, 50, 16);
    memory += (16 - ((int)memory & 15)) & 15;
    g_pAnimatedVolumeAllocator.Init(memory, 64, 50, 16);
    g_pAnimatedVolumeAllocator.ComputeNeededSize(64, 50, 16);
    g_pBoxList = 0;
    g_pGermanList = 0;
    g_pTankList = 0;
    g_pAnimatedMechList = 0;
    g_frameCounter = 3;
    g_delayDestroyHead = 0;
    g_delayDestroyTail = 0;
    for (int i = 0; i < 10; i++)
        {
        g_pDelayDestroyList[i].frame = 0;
        g_pDelayDestroyList[i].object = 0;
    }
    g_numActiveSoldiers = 0;
    TLTResource *resource = TLT_FindNextResourceByType((TLTResourceID)27, 0);
    g_maxActiveSoldiers = *(int *)(*(char **)(*(char **)((char *)resource + 8) + 184) + 8);
    g_iNumGermansQueuedThisFrame = 0;
    g_iGermanCreationQueueHeadIndex = 0;
    g_iGermanCreationQueueTailIndex = 0;
    for (int j = 0; j < 15; j++) {
        g_pGermanCreationQueue[j].trigger = 0;
        g_pGermanCreationQueue[j].index = -1;
        g_pGermanCreationQueue[j].value08 = 0;
        g_pGermanCreationQueue[j].wait = 0;
    }
    resource = TLT_FindNextResourceByType((TLTResourceID)25, 0);
    g_fDumpObjectCollisionRadius = *(float *)(*(char **)(*(char **)((char *)resource + 8) + 144) + 8);
    g_DumpObjectContainer.Reset();
    g_iNumberOfEnemiesSpawned = 0;
}

void ShutdownObjectCreators() {
    while (g_delayDestroyHead != g_delayDestroyTail)
        DestroySingleQueueElement();
    CDumpObjectContainer *container = &g_DumpObjectContainer;
    container->~CDumpObjectContainer();
    // The original tests this address before the placement new.
    if (container)
        new (container) CDumpObjectContainer;
    g_pBoxList = 0;
    g_pGermanList = 0;
    g_pTankList = 0;
    g_pAnimatedMechList = 0;
}

void UpdateObjectCreation() {
    while (g_delayDestroyHead != g_delayDestroyTail &&
           g_frameCounter - g_pDelayDestroyList[g_delayDestroyHead].frame > 1)
        DestroySingleQueueElement();
    int waited = 0;
    for (int i = g_iGermanCreationQueueHeadIndex; i != g_iGermanCreationQueueTailIndex;) {
        if (++g_pGermanCreationQueue[i].wait > 6)
            waited++;
        i++;
        i = i > 14 ? 0 : i;
    }
    NonVisDestructFarthestSoldiers(g_iNumGermansQueuedThisFrame + waited);
    g_iNumGermansQueuedThisFrame = 0;
    ProcessCreationQueue();
    g_frameCounter++;
}

void ProcessCreationQueue() {
    while (g_iGermanCreationQueueHeadIndex != g_iGermanCreationQueueTailIndex) {
        bool created = false;
        BSGO_Soldier *german = CreateGerman(g_pGermanCreationQueue[g_iGermanCreationQueueHeadIndex].trigger,
                                             g_pGermanCreationQueue[g_iGermanCreationQueueHeadIndex].index,
                                             g_pGermanCreationQueue[g_iGermanCreationQueueHeadIndex].value08, false, &created);
        if (!german)
            break;
        BSObjectTriggerEvent(g_pGermanCreationQueue[g_iGermanCreationQueueHeadIndex].trigger->scriptObject, 232,
                             german->script, 0, false);
        g_pGermanCreationQueue[g_iGermanCreationQueueHeadIndex].trigger = 0;
        g_pGermanCreationQueue[g_iGermanCreationQueueHeadIndex].index = -1;
        g_pGermanCreationQueue[g_iGermanCreationQueueHeadIndex].value08 = 0;
        g_pGermanCreationQueue[g_iGermanCreationQueueHeadIndex].wait = 0;
        g_iGermanCreationQueueHeadIndex++;
        g_iGermanCreationQueueHeadIndex = g_iGermanCreationQueueHeadIndex > 14 ? 0 : g_iGermanCreationQueueHeadIndex;
    }
}

int *CreateObject(TriggerObject *trigger, int index, int type, int value, void *data) {
    int *object = 0;
    switch (type) {
    case 0:
        break;
    case 1:
        object = (int *)CreateMechanic(trigger, index, value, data);
        break;
    case 11:
        object = (int *)CreateBasicAnimatedMech(trigger, index, value);
        break;
    case 2:
        object = (int *)CreateEnvModMechanic(trigger, index);
        break;
    case 3:
        object = (int *)CreateEnemy(trigger, index, value);
        break;
    case 4:
        object = (int *)CreatePowerup(trigger, index, value);
        break;
    case 7:
        object = (int *)CPSManager::Create(trigger, value, true, (eParticleSystem)1);
        break;
    case 8:
        object = (int *)CPSManager::Create(trigger, value, true, (eParticleSystem)2);
        break;
    case 10:
        object = (int *)CProjector::Create(value, true);
        break;
    case 9:
        object = (int *)CPSManager::Create(trigger, value, true, (eParticleSystem)3);
        break;
    }
    return object;
}

void *CreateEnemy(TriggerObject *trigger, int index, int value) {
    void *enemy;
    if (strcmp(trigger->GetChildScript(), "BOT3") == 0)
        enemy = CreateBots(trigger);
    else
        enemy = CreateGerman(trigger, index, value, true, 0);
    BotSetup();
    return enemy;
}
