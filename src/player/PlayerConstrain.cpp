// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "Capsule.h"
#include "Bullet.h"
extern "C" float sqrtf(float);
class CClsnConstraintsBase {
  public:
    int SatisfyConstraints(CMatrix &, float, float, float);
};
void CPlayerObject::UpdateCollisionVolumes() {
    ((CVolCapsule *)worldVolume)->CVolCapsule::TransformedCopy(*(IVolume *)localVolume, localToWorld);
    CVector3 minimum, maximum;
    ((CVolCapsule *)worldVolume)->CVolCapsule::GetExtents(minimum, maximum);
    extentMinX->value = minimum.x;
    extentMinY->value = minimum.y;
    extentMaxX->value = maximum.x;
    extentMaxY->value = maximum.y;
}
static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline void TransformDirection(CVector3 &out, const CMatrix &m, const CVector3 &v) {
    CVector3 r;
    r.x = v.x * m.row[0].x + v.y * m.row[1].x + v.z * m.row[2].x;
    r.y = v.x * m.row[0].y + v.y * m.row[1].y + v.z * m.row[2].y;
    r.z = v.x * m.row[0].z + v.y * m.row[1].z + v.z * m.row[2].z;
    out = r;
}
int CPlayerObject::Constrain(float scale) {
    unsigned int f = *(unsigned int *)&flags;
    f &= ~0x100000;
    *(unsigned int *)&flags = f;
    if (flags.bit14) return 1;
    if (pathBits.meleeImmune) return 1;
    if (pathFlags & 0x200) return 1;
    CVector3 start = localToWorld.GetPos();
    float one = 1.0f;
    float weight = pathBits.peeking ? 0.0f : 1.0f;
    float other = pathBits.peeking ? 0.0f : 1.0f;
    int satisfied = ((CClsnConstraintsBase *)constraints)->SatisfyConstraints(localToWorld, one, weight, other);
    flags.bit20 = satisfied ^ 1;
    CVector3 moved;
    CVector3 end = localToWorld.GetPos();
    SetSub(moved, end, start);
    moved *= 1.0f / field_10c8;
    float speed = sqrtf(moved.x * moved.x + moved.y * moved.y + moved.z * moved.z);
    if (!(speed <= 0.3f)) {
        float s = 0.3f / speed;
        moved *= s;
        moved *= field_10c8;
        start += moved;
        localToWorld.SetPos(start);
    }
    if (!(velocity.z <= 0.0f) && !(moved.z >= 0.0f)) velocity.z = 0.0f;
    if (flags.bit20) {
        velocity.Zero();
        acceleration.Zero();
    } else {
        CMatrix inverseTM;
        if (!CMatrix::s_ClassInit) CMatrix::InitClass();
        inverseTM.FastInverse(localToWorld);
        TransformDirection(acceleration, inverseTM, velocity);
    }
    return satisfied;
}
