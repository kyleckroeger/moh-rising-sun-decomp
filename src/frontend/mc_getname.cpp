// CMCDevice: one memory-card slot; see docs/Frontend.md.
namespace Realmc { class Interface; }
#include "MemoryCard.h"
struct CMemoryCard::Device {
    virtual void Slot0(); // Scoped: only the table position is established.
};
class CMCDevice : public CMemoryCard::Device {
  public:
    CMCDevice();
    const char *GetName() const;
    int GetIndex() const;
    void Invalidate(bool);
    void Cache(bool);
    int IsAvailable() const;
    int WasJustInserted() const;
    int WasJustRemoved() const;
    static int GetCount();
    static CMCDevice *GetDevice(unsigned int);
    Realmc::Interface *iface;    // +0x004
    unsigned short port;         // +0x008
    unsigned short unknown_a;    // +0x00a
    int needsCache;              // +0x00c
    char name[0x98 - 0x10];      // +0x010
    int justInserted;            // +0x098
    int justRemoved;             // +0x09c
};
extern CMCDevice gs_cards[];
extern CMemoryCard *g_hMemoryCard;

const char *CMCDevice::GetName() const { return name; }
int CMCDevice::GetIndex() const { return this - gs_cards; }
void CMCDevice::Invalidate(bool removed) {
    justInserted = 0;
    justRemoved = removed;
    needsCache = 1;
}
