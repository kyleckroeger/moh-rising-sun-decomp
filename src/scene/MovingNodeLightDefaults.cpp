#include "SceneNode.h"
void IMovingSceneNode::ApplyForceTo(const CVector3 &, float) {}
void IMovingSceneNode::EnterLightVolume(BPDLightVolume *) {}
void IMovingSceneNode::ExitLightVolume(BPDLightVolume *) {}
BPDLightVolume *IMovingSceneNode::GetLightVolume() { return 0; }
void IMovingSceneNode::SetLightVolumeTransitionDuration(float) {}
const char *IMovingSceneNode::GetName() { return 0; }
int IMovingSceneNode::GetTeam() { return 0; }
unsigned char IMovingSceneNode::GetPlayerIndex() { return 0; }
