// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
void CAnimatedPlayer::EnableAimModifier(bool enabled) { aimModifier.SetEnabled(enabled); }
int CAnimatedPlayer::LineAlmostCollided(CBullet *) const {}
void CAnimatedPlayer::UpdateAI(float) {}
CAnimatedPlayer *CAnimatedPlayer::AsAnimatedPlayerObject() { return this; }
const CAnimatedPlayer *CAnimatedPlayer::AsAnimatedPlayerObject() const { return this; }
void CAnimatedPlayer::SetScript(BSObject *value) { script = value; }
