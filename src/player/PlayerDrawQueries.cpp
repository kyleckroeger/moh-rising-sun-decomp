// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
// Only the field IsVisible writes; the complete class is unknown.
class CDrawContext {
  public:
    unsigned char unknown_0[0x100];
    int field_100;
};
void CPlayerObject::GetTMLocalToWorld(CMatrix &m) const { m = localToWorld; }
int CPlayerObject::IsVisible(CDrawContext &context) const {
    if (drawData) drawView->Submit(drawData, localToWorld);
    context.field_100 = 63;
    return 1;
}
unsigned int CPlayerObject::IsDrawEnabled() const { return 1; }
EClsnId CPlayerObject::GetCollisionId() const { return collisionId; }
