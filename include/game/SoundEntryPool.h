// AI-assisted prefix view through the free-entry link; see docs/Sound.md.
// No complete pool allocation or constructor is reproduced.
#ifndef GAME_SOUND_ENTRY_POOL_H
#define GAME_SOUND_ENTRY_POOL_H
#include "DwiList.h"
class SoundEntry;
// A free slot overlays the storage of a destroyed entry.
struct SoundFreeEntry {
    SoundFreeEntry *next;
    SoundFreeEntry(SoundFreeEntry *p) : next(p) {}
};
class SoundEntryPool {
  public:
    dwi::list<SoundEntry *> entries;
    // Descriptive inline accessor, not a recovered historical name.
    dwi::list<SoundEntry *> &GetEntries() { return entries; }
    const dwi::list<SoundEntry *> &GetEntries() const { return entries; }
    unsigned char unknown_20[0x12e0 - 0x20]; // Original slot storage stays opaque.
    SoundFreeEntry *freeEntries;
    // Descriptive helper: begin a free-link lifetime after entry destruction.
    void Recycle(SoundEntry *entry) {
        freeEntries = new (entry) SoundFreeEntry(freeEntries);
    }
    void Clear();
    void AddEntry(const SoundEntry &);
};
#endif
