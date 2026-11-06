#ifndef BPD_LOADER_INTERNAL_H
#define BPD_LOADER_INTERNAL_H
// Original method-only/static interfaces (AIPathFinding is a namespace, see
// AISplinePath.h). No class allocations are inferred.
#include "BPD.h"
#include "AIFilterGlobal.h"
namespace AIPathFinding {
    void ImportAreaPathNodeNetwork(BPDHeader *);
}
class CBuddyLeadBehavior {
public:
    static void CacheBuddyLeadTriggers();
};
class CMountedMachineGunBehavior {
public:
    static void DeregisterMMGTriggers();
    static void RegisterMMGTriggers();
};
void *TLT_LoadFileFromLevelBigFile(const char *, int *);
void InitAllTriggers();
void ClearMGTriggerObjects();
void FindAllMGPointsInBPDFile();
extern BPDHeader *g_pBPDHeader;
extern int g_pNumTriggers;
extern void *g_pRawPropertyData;
extern BPDLightVolume *g_pLightVolumes;
#endif
