// CScreen::GetVSyncs records and returns the retrace count.
#include "Screen.h"
extern "C" unsigned int VIGetRetraceCount(void);
int CScreen::GetVSyncs() {
    m_sFrameNum = VIGetRetraceCount();
    return m_sFrameNum;
}
