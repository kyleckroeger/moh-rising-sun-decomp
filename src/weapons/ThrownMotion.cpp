// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Thrown.h"
#include "IDestructible.h"
extern "C" float sqrtf(float);
void DeleteSpecialThrownObject(CThrownObject *);
void CThrownObject::SetVelocity(CVector3 &value) {
    velocity = value;
    LimitSpeed();
    lastVelocity = velocity;
}
void CThrownObject::AddVelocity(CVector3 &value) {
    velocity += value;
    LimitSpeed();
    lastVelocity = velocity;
}
void CThrownObject::LimitSpeed() {
    if (field_20c != -1) {
        IVolume *volume = worldVolume;
        float speed = sqrtf(velocity.LengthSquared());
        float limit = volume->GetRadius() * 2.0f;
        if (!(limit >= speed)) velocity *= limit / speed;
    }
}
void CThrownObject::MarkForDestruction(int delay) {
    if (!(thrownFlags & 0x4000000)) {
        CStaticObject::MarkForDestruction(delay);
        thrownFlags |= 0x4000000;
    }
}
void CThrownObject::Destroy() {
    thrownFlags &= ~0x4000000;
    DeleteSpecialThrownObject(this);
}
void CThrownObject::IGO_Recycle() { ((IDestructible *)this)->MarkForDestruction(1); }
int CThrownObject::GetObjectType() const { return 4; }
CThrownObject *CThrownObject::AsThrownObject() { return this; }
void CThrownObject::TranslateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum) {}
void CThrownObject::RotateFromScript(const CVector3 &, const CVector3 &, EBSEventEnum) {}
void CThrownObject::ScaleFromScript(float, float, EBSEventEnum) {}
