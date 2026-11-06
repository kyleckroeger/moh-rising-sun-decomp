// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
class CVolBox {
  public:
    void SetCorners(const CVector3 &, const CVector3 &);
};
// Only the fields the player's Draw compares; names are descriptive.
struct DrawViewOwner { unsigned char unknown_0[24]; int ownerIndex; };
class CDrawContext {
  public:
    unsigned char unknown_0[276];
    DrawViewOwner *view;
};
void CAnimatedPlayer::Draw(CDrawContext &context) {
    if (context.view->ownerIndex != *(const int *)&player->viewWordHigh) ((CSoldierObject *)this)->Draw(context);
}
void CAnimatedPlayer::UpdateAnimation(float elapsed) { ((CAnimObject *)this)->UpdateAnimation(elapsed); }
void CAnimatedPlayer::SetPlayer(CPlayerObject *value) { player = value; }
void CAnimatedPlayer::SetFlocoParams(float rate, int count) {
    flocoRate = rate;
    flocoCount = count;
}
void CAnimatedPlayer::GetFlocoParams(float &rate, int &count) {
    rate = flocoRate;
    count = flocoCount;
}
int CAnimatedPlayer::GetLocalBoundingVolume(ISceneNode::EVolumeType type) const {
    switch (type) {
    case 3:
        return (int)localBox;
    case 0:
    case 1:
    case 2:
        return (int)localVolume;
    case 4:
        return 0;
    default:
        return 0;
    }
}
int CAnimatedPlayer::GetWorldBoundingVolume(ISceneNode::EVolumeType type) const {
    switch (type) {
    case 3:
        return (int)worldBox;
    case 0:
    case 1:
    case 2:
        return (int)worldVolume;
    case 4:
        return 0;
    default:
        return 0;
    }
}
void CAnimatedPlayer::UpdateLocalBoundingVolume(ISceneNode::EVolumeType type, const CVector3 &min, const CVector3 &max) {
    if (type == 3) localBox->SetCorners(min, max);
}
