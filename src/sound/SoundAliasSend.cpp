// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundEvents.h"
#include "SoundManager.h"
#include "SoundEntryPool.h"

// Original private SoundAliasEvent.cpp pool; its storage stays original context.
extern SoundEntryPool s_entries;

// Descriptive helper; the meanings of the two copied words remain unknown.
static inline void SetFields(SoundEntry &entry, int a, int b) {
    entry.field_84 = a;
    entry.field_88 = b;
}

int SoundAliasEvent::SendEvent() {
    int result = 0;
    s_entries.Clear();
    if (alias) {
        g_SoundMan.GetSoundEntries(s_entries, alias);
        dwi::list<SoundEntry *> &entries = s_entries.GetEntries();
        if (entries.begin() != entries.end()) {
            for (dwi::list<SoundEntry *>::iterator it = entries.begin(); it != entries.end(); it++) {
                (*it)->SetEmitter(emitter);
                if (field_08) {
                    SetFields(**it, field_08, field_1c);
                    for (int i = 0; i < 3; ++i)
                        (*it)->params[i] = field_10[i];
                }
            }
            result = g_SoundMan.GetSchedule().AddSounds(s_entries);
        }
    }
    return result;
}
