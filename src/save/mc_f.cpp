// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original MemoryCard.cpp.
#include "MemoryCard.h"

extern void *g_pTextDatabase;
extern void *g_pMiscTextDatabase;
extern void *g_pScratchpad;
extern int g_bWaitingForReply;
extern int g_bIsBusy;
extern "C" void MEM_free(void *);

class CMCDevice {
public:
    static int GetCount();
    static const CMemoryCard::Device *GetDevice(unsigned int index);
};

void CMemoryCard::Free(void *p) {
    MEM_free(p);
}

int CMemoryCard::GetDeviceCount() const {
    return CMCDevice::GetCount();
}

const CMemoryCard::Device *CMemoryCard::GetDevice(unsigned int index) const {
    return CMCDevice::GetDevice(index);
}
