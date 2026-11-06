// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "MPWeaponSet.h"
extern int g_bInMultiplayerMode;
float MathFunRandomReal(float, float);
static inline float SumOfSquares(const CVector3 &v) {
    float sum = v.x * v.x + v.y * v.y;
    sum += v.z * v.z;
    return sum;
}
extern float PHYS_RUNSPEED;
extern float MP_SPEED_SCALE;
void CPlayerObject::ShakeCamera(float &a, float &b, float dt) {
    if (cameraShake.IsActive() || backgroundShake.IsActive()) {
        cameraShake.Update(dt);
        backgroundShake.Update(dt);
        if (!(backgroundShake.current <= cameraShake.current)) backgroundShake.Evaluate(a, b);
        else cameraShake.Evaluate(a, b);
    }
    if (flags.motionShake) {
        float step = motionShakeAmount / motionShakeRate;
        float ratio;
        if (!g_bInMultiplayerMode) ratio = SumOfSquares(velocity) / (PHYS_RUNSPEED * PHYS_RUNSPEED);
        else ratio = SumOfSquares(velocity) / (PHYS_RUNSPEED * PHYS_RUNSPEED * MP_SPEED_SCALE * MP_SPEED_SCALE);
        if (!(ratio <= 1.0f)) ratio = 1.0f;
        ratio *= motionShakeAmount;
        if (!(ratio < motionShakeTime)) motionShakeTime += step;
        else motionShakeTime -= step;
        if (motionShakeTime > motionShakeAmount) motionShakeTime = motionShakeAmount;
        else if (!(motionShakeTime >= 0.0f)) motionShakeTime = 0.0f;
        float range = motionShakeTime * 0.25f;
        float low = -range;
        b += MathFunRandomReal(low, range);
        a += MathFunRandomReal(low, range);
    }
    if (!cameraShake.IsActive() && !backgroundShake.IsActive() && !flags.motionShake) {
        b = 0.0f;
        a = 0.0f;
    }
}
