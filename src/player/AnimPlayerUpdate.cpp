// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "SceneObjects.h"
#include "Bullet.h"
#include "Sphere.h"
extern int g_bInMultiplayerMode;
static inline void CopyMatrix(CMatrix &to, const CMatrix &from) { to = from; }
static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) {
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
}
static inline void SetLerp(CVector3 &r, const CVector3 &a, const CVector3 &b, float t) {
    float ax = a.x, ay = a.y, az = a.z;
    r.x = (b.x - ax) * t + ax;
    r.y = (b.y - ay) * t + ay;
    r.z = (b.z - az) * t + az;
}
static inline void PostMultiply(CMatrix &m, const CMatrix &o) {
    CMatrix::s_TempMat = m;
    m.Multiply(CMatrix::s_TempMat, o);
}
void CAnimatedPlayer::BeginUpdate(float dt) {
    ((CAnimObject *)this)->BeginUpdate(dt);
    lastPosition = transform.GetPos();
    lastYaw = yaw;
    CMatrix bone;
    CopyMatrix(bone, matrices[1]);
    boneRotation.SetFromMatrix(bone);
    bonePosition = bone.GetPos();
    velocity.Zero();
}
void CAnimatedPlayer::AttemptUpdate(float dt) {
    ((CAnimObject *)this)->AttemptUpdate(dt);
    CVector3 move;
    SetScaled(move, velocity, dt);
    CMatrix bone;
    CopyMatrix(bone, matrices[1]);
    CQuaternion rotation;
    rotation.SetFromMatrix(bone);
    CVector3 position = bone.GetPos();
    SetLerp(nextBonePosition, bonePosition, position, dt);
    nextBoneRotation.Slerp(rotation, boneRotation, dt);
    nextBoneRotation.GetMatrix(bone);
    bone.SetPos(nextBonePosition);
    if (!attached) {
        float angle = 0.0f;
        yaw = angle;
        transform.BuildRotZ(angle);
        transform.SetPos(lastPosition);
        transform.Translate(move);
    } else if (attachedToLocation) {
        transform = attachLocation;
    }
    if (localVolume) {
        PostMultiply(bone, transform);
        {
            CVector3 origin(0.0f, 0.0f, 0.0f);
            sphere->center = origin;
        }
        ((IVolume *)worldVolume)->TransformedCopy(*(IVolume *)localVolume, bone);
        CVector3 minimum, maximum;
        ((IVolume *)worldVolume)->GetExtents(minimum, maximum);
        extentMinX->value = minimum.x;
        extentMinY->value = minimum.y;
        extentMaxX->value = maximum.x;
        extentMaxY->value = maximum.y;
    } else {
        CVector3 location;
        ((ISceneNode *)this)->GetPosition(location);
        extentMinX->value = location.x;
        extentMinY->value = location.y;
        extentMaxX->value = location.x;
        extentMaxY->value = location.y;
    }
    field_220 = 0;
    records[0].Clear();
    records[1].Clear();
    records[2].Clear();
    CWeapon *current = player->GetCurrentWeapon();
    if (player->flags.headingOverride) field_1080 = 0;
    else field_1080 = 1;
    if (current) {
        int slot = currentWeapon;
        if (weapons[slot]) {
            if (player->flags.headingOverride) {
                weapons[slot]->muzzleFlashStatus = 0;
                if (!g_bInMultiplayerMode) ((ISceneNode *)weapons[currentWeapon])->SetDrawEnabled(false);
            } else {
                weapons[slot]->muzzleFlashStatus = current->GetMuzzleFlashStatus();
            }
        }
    }
}
