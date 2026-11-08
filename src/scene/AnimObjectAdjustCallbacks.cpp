// AI-assisted reconstruction from GR8E69. Part of the original anim_obj.cpp; member names are
// descriptive.
#include "SceneObjects.h"
#include "../game-containers/TargetTypes.h"
extern "C" float atan2f(float, float);

static inline void SetScaled(CVector3 &r, const CVector3 &v, float s) {
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
}

void CAnimObject::Adjusting_cb(const float &time) {
    CVector3 offset(adjustTarget);
    float turn = adjustValue - yawCopy;
    offset -= previousPosition;
    float low = -3.1415927410125732f, high = 3.1415927410125732f;
    if (!(turn <= high)) {
        do {
            turn -= 6.2831854820251465f;
        } while (turn > high);
    }
    if (!(turn >= low)) {
        do {
            turn += 6.2831854820251465f;
        } while (turn < low);
    }
    float remaining = callbackLast;
    if (!(remaining <= 0.0f)) {
        float step = time;
        if (!(step <= remaining))
            step = remaining;
        float left = remaining - step;
        callbackLast = left;
        float share = left / callbackStart;
        animMoveScale = share;
        float scale = (1.0f - share) * (step / remaining);
        SetScaled(animMove, offset, scale);
        if (adjustBlend)
            yawRate = turn * scale;
    } else {
        animMoveScale = 0.0f;
        animMove = offset;
        if (adjustBlend)
            yawRate = turn;
        animCallback = -1;
    }
}

void CAnimObject::TrackTarget_cb(const float &) {
    if (*(WeakPtr<ISceneNode, 8> *)weakNode) {
        CVector3 position;
        (*(WeakPtr<ISceneNode, 8> *)weakNode)->GetPosition(position);
        position -= transform.GetPos();
        float turn = atan2f(-position.x, position.y) - yaw;
        float low = -3.1415927410125732f, high = 3.1415927410125732f;
        if (!(turn <= high)) {
            do {
                turn -= 6.2831854820251465f;
            } while (turn > high);
        }
        if (!(turn >= low)) {
            do {
                turn += 6.2831854820251465f;
            } while (turn < low);
        }
        yawRate = turn;
    }
}
