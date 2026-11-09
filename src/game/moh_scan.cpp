// AI-assisted reconstruction from GR8E69. Part of the original Moh3.cpp. The scan task's interface
// is reached only through its table here; the Execute slot is confirmed by this class's table.
#include "MemoryCard.h"

struct CMemoryCard::ScanTask {
    virtual ~ScanTask();
    virtual void Execute(const char *, unsigned int) = 0;
};

class OnePassScanTask : public CMemoryCard::ScanTask {
  public:
    virtual ~OnePassScanTask();
    virtual void Execute(const char *, unsigned int);
    int done; // +0x04
};

void OnePassScanTask::Execute(const char *, unsigned int) {
    done = 1;
}
