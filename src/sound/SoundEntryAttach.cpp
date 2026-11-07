// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundManager.h"
#include "SoundEvents.h"
void SoundEntry::SetEmitter(const SoundEmitter *p) {
    if (p) {
        emitter = (SoundEmitter *)emitterStorage;
        p->Clone(emitter);
    } else emitter = 0;
}
// Descriptive helper; the original inline name is unknown.
static inline int ShouldNotify(const SoundEntry &entry, SoundCBEventCode code) {
    if (entry.field_84) return (code & entry.field_88) != 0;
    return 0;
}
void SoundEntry::DoNotifyCallback(SoundCBEventCode code) {
    if (ShouldNotify(*this, code))
        ((void (*)(SoundEntry *, SoundCBEventCode))field_84)(this, code);
}
