// AI-assisted reconstruction from GR8E69; see docs/Sound.md.
#include "SoundEvents.h"

int SoundEvent::UpdateEvent(unsigned int) { return 0; }
int SoundEvent::EndEvent(unsigned int) { return 0; }
void SoundEvent::SetEmitter(SoundEmitter *p) { emitter = p; }
void SoundEvent::SetListener(Listener *p) { listener = p; }
