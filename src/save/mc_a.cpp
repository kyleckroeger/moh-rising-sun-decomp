// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original MemoryCard.cpp.
#include "MemoryCard.h"

extern void *g_pTextDatabase;
extern void *g_pMiscTextDatabase;
extern void *g_pScratchpad;
extern int g_bWaitingForReply;
extern int g_bIsBusy;
extern "C" int MEM_free(void *);

extern "C" void LOCALE_create(void *, int);
extern "C" void LOCALE_setstate(void *, int, int);
void *TLT_LoadFileNormal(const char *, int *, int);

CMemoryCard::CMemoryCard() {
    g_pTextDatabase = TLT_LoadFileNormal("data/SaveGame/SaveGame.loc", 0, 0);
    g_pMiscTextDatabase = TLT_LoadFileNormal("data/SaveGame/sg_misc.loc", 0, 0);
    LOCALE_create(g_pTextDatabase, 1);
    LOCALE_create(g_pMiscTextDatabase, 1);
    LOCALE_setstate(g_pTextDatabase, 0, 0);
}
