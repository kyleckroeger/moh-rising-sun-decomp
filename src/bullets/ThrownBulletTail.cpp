// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "Fiber.h"
extern "C" float sqrtf(float);
#include "IDestructible.h"
void CThrownBullet::Explode() { explodeNow = caught ^ 1; }
void CThrownBullet::SetDetonationTime(float time) {
    if (properties->IsTimed()) field_b0 = time;
}
void CThrownBullet::HandleBulletCollision(CBullet *bullet, const CCollision &) {
    if (bullet->AsProjectile()) Explode();
}
void CThrownBullet::Halt() {
    velocity.Zero();
    spin.Zero();
}
void CThrownBullet::ApplyForceTo(const CVector3 &direction, float amount) {
    float scale = amount * 0.001953125f;
    velocity = direction;
    float length = sqrtf(velocity.LengthSquared());
    if (length != 0.0f) velocity *= 1.0f / length;
    velocity *= properties->weight * scale;
    spin = velocity;
}
void CThrownBullet::SomebodyCaughtYou() {
    if (explodeNow) return;
    if (noCatch) return;
    explodeNow = 0;
    caught = 1;
    ((IDestructible *)this)->MarkForDestruction(0);
}
void CThrownBullet::EnterLightVolume(BPDLightVolume *volume) { lightVolumes.AddVolume(volume); }
void CThrownBullet::ExitLightVolume(BPDLightVolume *volume) { lightVolumes.RemoveVolume(volume); }
BPDLightVolume *CThrownBullet::GetLightVolume() { return lightVolumes.GetVolume(); }
