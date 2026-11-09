// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "ScriptTimers.h"
extern int g_bInMultiplayerMode;
float MathFunNormalizeAngleNegativePiToPi(float);
static inline float fabsf(float x) { return __builtin_fabsf(x); }
void CAnimatedPlayer::PostUpdate(float dt) {
    if (!g_bInMultiplayerMode || !player->pathBits.meleeImmune) {
        if (!(flocoRate >= 0.001f)) {
            float heading = player->GetHeading();
            float base = flocoHeading;
            float delta = MathFunNormalizeAngleNegativePiToPi(heading - base);
            float distance = fabsf(delta);
            if (distance > 0.7853982f || turning) {
                if (!turning) {
                    turning = 1;
                    BSObjectTriggerEvent(script, 299, 0, 0, false);
                }
                float direction;
                if (!(delta >= 0.0f)) direction = -1.0f;
                else direction = 1.0f;
                base += direction * (dt * 0.03926991f);
                if (!(distance <= 0.7853982f)) base += (distance - 0.7853982f) * direction;
                if (!(MathFunNormalizeAngleNegativePiToPi(heading - base) * direction >= 0.0f)) {
                    base = heading;
                    BSObjectTriggerEvent(script, 300, 0, 0, false);
                    turning = 0;
                }
                flocoHeading = base;
            }
        } else {
            BSObjectTriggerEvent(script, 300, 0, 0, false);
            turning = 0;
            flocoHeading = player->GetHeading();
        }
    } else {
        BSObjectTriggerEvent(script, 300, 0, 0, false);
        turning = 0;
        flocoHeading = player->field_e2c;
    }
    CVector3 position = transform.GetPos();
    transform.BuildRotZ(flocoHeading);
    transform.Translate(position);
}
