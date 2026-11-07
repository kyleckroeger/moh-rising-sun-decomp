// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundEntryPool.h"
void SoundEntryPool::Clear() {
    dwi::list<SoundEntry *>::iterator it = entries.begin();
    while (it != entries.end()) {
        (*it)->~SoundEntry();
        Recycle(*it);
        it = entries.erase(it);
    }
}
void SoundEntryPool::AddEntry(const SoundEntry &entry) {
    SoundEntry *out = (SoundEntry *)freeEntries;
    freeEntries = freeEntries->next;
    entry.Clone(out);
    entries.push_back(out);
}
