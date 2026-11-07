// AI-assisted prefix view; see docs/Sound.md. No pool allocation is reproduced.
#ifndef GAME_SOUND_ENTRY_POOL_H
#define GAME_SOUND_ENTRY_POOL_H
#include "DwiList.h"
class SoundEntry;
class SoundEntryPool {
  public:
    dwi::list<SoundEntry *> entries;
    // Descriptive inline accessor, not a recovered historical name.
    dwi::list<SoundEntry *> &GetEntries() { return entries; }
    void Clear();
};
#endif
