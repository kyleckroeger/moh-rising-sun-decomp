// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundEntryPool.h"
extern int g_bDisableSound;
unsigned int SoundSchedule::AddSounds(const SoundEntryPool &pool) {
    unsigned int id = 0;
    if (g_bDisableSound) return 0;
    const dwi::list<SoundEntry *> &entries = pool.GetEntries();
    if (entries.count) id = AllocateEventID();
    for (dwi::list<SoundEntry *>::iterator it = entries.begin(); it != entries.end(); it++) {
        busses.RetroFitNewSound(*it);
        (*it)->eventID = id;
        if ((*it)->spatial) (*it)->Calculate3D();
        if (Add(*it) < 0) return 0;
    }
    return id;
}
