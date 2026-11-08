// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
// The inline helpers' names are descriptive; the local names are inferred.
#include "CameraShake.h"
#include "Camera.h"
#include "CLine3.h"
#include "Fiber.h"
#include "Capsule.h"
#include "CDBObject.h"
#include "SceneNode.h"
#include "Weapon.h"
extern int g_bInMultiplayerMode;
extern int g_bCoopMultiplayer;
extern float g_MPAnimMeshZOffset;
extern float STAND_TO_WALK_TRANSITION;
static inline float NormalizeV(CVector3 &v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length != 0.0f) {
        float inverse = 1.0f / length;
        v.x *= inverse;
        v.y *= inverse;
        v.z *= inverse;
    }
    return length;
}
static inline float NormalizeXY(CVector3 &v) {
    float length = sqrtf(v.x * v.x + v.y * v.y);
    if (length != 0.0f) {
        float inverse = 1.0f / length;
        v.x *= inverse;
        v.y *= inverse;
        v.z *= inverse;
    }
    return length;
}
static inline void SetCross(CVector3 &out, const CVector3 &a, const CVector3 &b) {
    CVector3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    out.Set(r.x, r.y, r.z);
}
#define ROW(i) { float x = a.row[i].x, y = a.row[i].y, z = a.row[i].z, w = a.row[i].w; \
    r.row[i].x = x * b.row[0].x + y * b.row[1].x + z * b.row[2].x + w * b.row[3].x; \
    r.row[i].y = x * b.row[0].y + y * b.row[1].y + z * b.row[2].y + w * b.row[3].y; \
    r.row[i].z = x * b.row[0].z + y * b.row[1].z + z * b.row[2].z + w * b.row[3].z; \
    r.row[i].w = x * b.row[0].w + y * b.row[1].w + z * b.row[2].w + w * b.row[3].w; }
static inline void Concat(CMatrix &r, const CMatrix &a, const CMatrix &b) {
    ROW(0) ROW(1) ROW(2) ROW(3)
}
static inline void AddZ(CVector3 &v, float d) { v.z += d; }
static inline void SubXY(CVector3 &v, const CVector3 &o) { v.x -= o.x; v.y -= o.y; }
static inline void ClearZ(CVector3 &v) { v.z = 0.0f; }
static inline float DotV(const CVector3 &a, const CVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline void InitSlot(CMatrix &m) { m.GetSlot(0); }
static inline void CopyMatrix(CMatrix &to, const CMatrix &from) { to = from; }
static inline CVector3 EndCopy(const CLine3 &l) { return l.end; }
static inline void SetZ(CVector3 &v, float z) { v.z = z; }
#define BODY ((IMovingSceneNode *)body)
#define CAMERA (*(CCamera *)&clip)
void CPlayerObject::CommitUpdate() {
    if (g_bInMultiplayerMode || g_bCoopMultiplayer) {
        if (flags.headingOverride) {
            CVector3 position = localToWorld.GetPos();
            AddZ(position, g_MPAnimMeshZOffset);
            BODY->SetPosition(position);
        } else if (!pathBits.meleeImmune) {
            CMatrix tm;
            if (!CMatrix::s_ClassInit) CMatrix::InitClass();
            InitSlot(tm);
            tm.Translate(localToWorld.GetPos());
            tm.Translate(CVector3(0.0f, 0.0f, g_MPAnimMeshZOffset));
            BODY->SetTMLocalToWorld(tm);
        }
        CVector3 direction = localToWorld.GetPos();
        direction -= position;
        SetZ(direction, 0.0f);
        float speed = NormalizeXY(direction);
        int type = 0;
        if (!(speed <= STAND_TO_WALK_TRANSITION)) {
            CVector3 front = localToWorld.GetFront();
            NormalizeV(front);
            float dot = DotV(front, direction);
            if (!(dot >= -0.7071f)) {
                type = 4;
            } else if (!(dot >= 0.7071f)) {
                ClearZ(front);
                SetZ(direction, 0.0f);
                SetCross(front, front, direction);
                if (!(front.z <= 0.0f)) type = 3;
                else type = 2;
            } else {
                type = 1;
            }
        }
        if (pathBits.mmgBlocked) ((CAnimatedPlayer *)body)->SetFlocoParams(0.0f, 0);
        else ((CAnimatedPlayer *)body)->SetFlocoParams(speed, type);
    }
    int blocked = pathBits.mmgBlocked;
    if (!blocked) {
        IVolume *world = (IVolume *)((ISceneNode *)g_scene.world)->GetWorldBoundingVolume((ISceneNode::EVolumeType)0);
        CVector3 start = localToWorld.GetPos();
        if (pathBits.peeking) start = peekBase;
        AddZ(start, 0.39f);
        CLine3 line(start, CVector3(start.x, start.y, -10000.0f));
        CVolFiber fiber(line);
        CCollision collision;
        CCDBObject::DisablePassThru(true);
        int result = world->TestCollision(fiber, collision, false);
        CCDBObject::DisablePassThru(false);
        static int toggle = 1;
        if (result == 1) {
            if (toggle) toggle = blocked;
            acceleration.Zero();
            velocity.Zero();
            pathBits.peekAllowed = 1;
            ((ISceneNode *)this)->AttemptUpdate(0.0f);
            return;
        }
        toggle = 1;
    } else {
        CMatrix tm;
        CopyMatrix(tm, localToWorld);
        const CMatrix &eye = cameraMatrix;
        CMatrix view;
        if (!CMatrix::s_ClassInit) CMatrix::InitClass();
        Concat(view, eye, tm);
        CAMERA.CCamera::SetTMLocalToWorld(view);
        float fovRadians = fov * 0.0174532942f;
        CAMERA.CCamera::SetPerspective(fovRadians, fovRadians * CCamera::ASPECT_4x3);
        CAMERA.CCamera::AttemptUpdate(1.0f);
        // The controller at +0x1ca0 is viewed as TargetFind elsewhere; the symbol names AimAdjust.
        ((AimAdjust *)&targetFind)->UpdateTarget(CAMERA);
        CPlayerWeaponObject *weapon = GetCurrentPlayerWeapon();
        if (!flags.hideWeapon && weapon) {
            weapon->UpdateWeaponTransforms();
            weapon->UpdateMuzzleFlash();
            if (thrownWeapon) {
                CVector3 forward, eye;
                CAMERA.CCamera::GetForward(forward);
                CAMERA.CCamera::GetPosition(eye);
                thrownWeapon->position = eye;
                thrownWeapon->direction = forward;
            }
        }
    }
    if (flags.bit20) {
        velocity.Zero();
        acceleration.Zero();
    }
    position = localToWorld.GetPos();
    const CVector3 &end = EndCopy(((CVolCapsule *)localVolume)->line);
    field_270 = end.z;
}
