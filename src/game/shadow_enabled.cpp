// CCharacterShadow's per-frame update and IsDrawEnabled.
#include "CharacterShadow.h"
void CCharacterShadow::Update(float) { AttachProjector(); }
unsigned int CCharacterShadow::IsDrawEnabled() const { return enabled; }
