// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp. Each class embeds script data at +0x14
// after the BSGO_Basic prefix; the scene node and proximity offsets differ per class.
#include "ObjCreate.h"

void *BSGO_Destructable::GetScriptData() { return scriptData; }
ISceneNode *BSGO_Destructable::GetSceneNode() { return binding.sceneNode; }
void *BSGO_Destructable::GetProximityData() { return proximityData; }

void *BSGO_Anim::GetScriptData() { return scriptData; }
ISceneNode *BSGO_Anim::GetSceneNode() { return binding.sceneNode; }
void *BSGO_Anim::GetProximityData() { return proximityData; }

void *BSGO_Soldier::GetScriptData() { return scriptData; }
ISceneNode *BSGO_Soldier::GetSceneNode() { return binding.sceneNode; }
void *BSGO_Soldier::GetProximityData() { return proximityData; }

void *BSGO_Tank::GetScriptData() { return scriptData; }
ISceneNode *BSGO_Tank::GetSceneNode() { return binding.sceneNode; }
void *BSGO_Tank::GetProximityData() { return proximityData; }

void *BSGO_Player::GetScriptData() { return scriptData; }
ISceneNode *BSGO_Player::GetSceneNode() { return sceneNode; }
void *BSGO_Player::GetProximityData() { return proximityData; }
