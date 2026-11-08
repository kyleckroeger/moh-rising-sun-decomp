// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original anim_obj.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "CQuaternion.h"
#include "Sphere.h"
#include "Volume.h"

extern int BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);

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

static inline void ScaleRows(CMatrix &m, float s) {
    m.row[0].x *= s;
    m.row[0].y *= s;
    m.row[0].z *= s;
    m.row[1].x *= s;
    m.row[1].y *= s;
    m.row[1].z *= s;
    m.row[2].x *= s;
    m.row[2].y *= s;
    m.row[2].z *= s;
    m.row[3].x *= s;
    m.row[3].y *= s;
    m.row[3].z *= s;
}

void CAnimObject::AttemptUpdate(float dt) {
    CVector3 move;
    SetScaled(move, velocity, dt);
    float turn = yawRate * dt;
    CMatrix bone;
    CopyMatrix(bone, JointMatrices()[1]);
    animStep = dt;
    float t = dt * animRate;
    if (!(t <= 1.0f))
        t = 1.0f;
    else if (!(t >= 0.0f))
        t = 0.0f;
    CQuaternion rotation;
    rotation.SetFromMatrix(bone);
    CVector3 position = bone.GetPos();
    SetLerp(pendingOffset, committedOffset, position, t);
    pendingRotation.Slerp(rotation, committedRotation, t);
    pendingRotation.GetMatrix(bone);
    bone.SetPos(pendingOffset);
    if (!(transform.row[3].z >= -250.0f))
        BSObjectTriggerEvent(script, 42, 0, 0, false);
    if (!attached) {
        float angle = yawCopy + turn;
        float low = -3.1415927f;
        float high = 3.1415927f;
        if (!(angle <= high)) {
            do {
                angle -= 6.2831855f;
            } while (angle > high);
        }
        if (!(angle >= low)) {
            do {
                angle += 6.2831855f;
            } while (angle < low);
        }
        yaw = angle;
        transform.BuildRotZ(angle);
        ScaleRows(transform, scale);
        transform.SetPos(previousPosition);
        transform.Translate(move);
    } else if (attachedToLocation) {
        if (!(attachBlend <= 0.0f)) {
            float k = 1.0f / attachBlend;
            CVector3 delta;
            CVector3 from = transform.GetPos();
            CVector3 to = attachLocation.GetPos();
            delta.x = (to.x - from.x) * k;
            delta.y = (to.y - from.y) * k;
            delta.z = (to.z - from.z) * k;
            from.x += delta.x;
            from.y += delta.y;
            from.z += delta.z;
            transform = attachLocation;
            transform.SetPos(from);
            attachBlend -= dt;
        } else {
            transform = attachLocation;
        }
    }
    if (localVolumeType0) {
        PostMultiply(bone, transform);
        if (animatedVolume) {
            CVector3 origin(0.0f, 0.0f, 0.0f);
            ((CVolSphere *)animatedVolume)->center = origin;
        }
        worldVolumeType0->TransformedCopy(*localVolumeType0, bone);
        CVector3 minimum, maximum;
        worldVolumeType0->GetExtents(minimum, maximum);
        sortMinX->value = minimum.x;
        sortMinY->value = minimum.y;
        sortMaxX->value = maximum.x;
        sortMaxY->value = maximum.y;
    } else {
        CVector3 location;
        ((ISceneNode *)this)->GetPosition(location);
        sortMinX->value = location.x;
        sortMinY->value = location.y;
        sortMaxX->value = location.x;
        sortMaxY->value = location.y;
    }
    hurtling = 0;
}
