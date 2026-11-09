// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "Camera.h"
#include "CLine3.h"
#include "Volume.h"
#include "Fiber.h"
#include "BSGameObject.h"
#include "ObjCreate.h"
#include "AIDoodad.h"
#include "AITargeting.h"
#include "CDBObject.h"
#include "ObserverTypes.h"
#include "SceneNode.h"
extern "C" float sqrtf(float);
static inline void SetSub(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
static inline void SetSum(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
}
void CPlayerObject::AimAdjust::UpdateTarget(const CCamera &camera) {
    if (!active) return;
    const float reach = 200.0f;
    CVector3 offset;
    camera.GetForward(offset);
    offset *= reach;
    CVector3 eye;
    camera.GetPosition(eye);
    CVector3 end = eye;
    end += offset;
    CLine3 line(eye, end);
    if (!subject) {
        ISubject *ally = 0;
        const float nearestT = 0.005f;
        float bestEnemy = 10.0f;
        float bestAlly = bestEnemy;
        BSGO_Basic *head = (BSGO_Basic *)g_pGermanList;
        if (head) for (BSGO_Basic *object = head; object; object = object->next) {
            CSoldierObject *soldier = (CSoldierObject *)object->GetSceneNode();
            if (!soldier) continue;
            int friendly = 1;
            if (((ISceneNode *)soldier)->GetAIObject()->team == 2) friendly = 0;
            if (((ISceneNode *)soldier)->GetAIObject()->GetAIStatusVar((aistatus_var)0) <= 0) continue;
            CVector3 position;
            ((ISceneNode *)soldier)->GetPosition(position);
            CVector3 nearest;
            float t;
            line.GetProjection(position, nearest, t);
            if (t < nearestT || t > 1.0f) continue;
            position -= nearest;
            float distance = sqrtf(position.x * position.x + position.y * position.y + position.z * position.z);
            if (friendly) {
                if (distance >= bestAlly) continue;
                bestAlly = distance;
                ally = (ISubject *)soldier;
                field_40 = 1;
                target = nearest;
            } else {
                if (distance >= bestEnemy) continue;
                bestEnemy = distance;
                link.Unlink();
                subject = (ISubject *)soldier;
                ((ISubject *)soldier)->AddObserver((IObserver *)this);
                field_40 = 1;
                target = nearest;
            }
        }
        if (!subject && ally) {
            link.Unlink();
            subject = ally;
            ally->AddObserver((IObserver *)this);
        }
    }
    if (!field_40) {
        IVolume *world = (IVolume *)((ISceneNode *)g_scene.world)->GetWorldBoundingVolume((ISceneNode::EVolumeType)0);
        CVolFiber fiber(line);
        CCollision collision;
        CCDBObject::DisablePassThru(true);
        int result = world->TestCollision(fiber, collision, false);
        CCDBObject::DisablePassThru(false);
        if (result != 1) offset *= collision.depth;
        else offset *= 0.25f;
        SetSum(target, eye, offset);
        lastTarget = target;
        field_40 = 1;
    } else {
        int zero;
        CVector3 position = lastTarget;
        float t;
        CVector3 *aim;
        if (HasSubject()) {
            if (((ISceneNode *)subject)->GetAIObject()->GetAIStatusVar((aistatus_var)0) <= 0) {
                zero = 0;
                link.Unlink();
                field_40 = zero;
                goto clear;
            }
            ((ISceneNode *)subject)->GetPosition(position);
        }
        aim = &target;
        line.GetProjection(position, *aim, t);
        if (!(t < 0.005f) && !(t > 1.0f)) {
            CVector3 away;
            SetSub(away, position, *aim);
            if (away.x * away.x + away.y * away.y + away.z * away.z <= 400.0f) return;
        }
        zero = 0;
        field_40 = zero;
        link.Unlink();
    clear:
        subject = (ISubject *)zero;
    }
}
