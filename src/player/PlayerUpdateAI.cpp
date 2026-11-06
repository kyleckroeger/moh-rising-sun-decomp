// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "AITargeting.h"
void CPlayerObject::UpdateAI(float elapsed) { ((CAIObjectInterface *)aiObject)->Update(elapsed); }
