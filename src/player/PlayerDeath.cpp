// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "SceneObjects.h"
extern "C" float cosf(float);
extern "C" float atan2f(float, float);
extern int g_bInMultiplayerMode;
extern int g_bCoopMultiplayer;
void SoundSetMasterVolume(int);
void CPlayerObject::StartDeathSequence() {
    if (field_dc0 != 0.0f) return;
    collisionId = (EClsnId)25;
    flags.headingOverride = 1;
    if (currentWeapon) {
        currentWeapon->Shoot((EWeaponShootType)1, 0);
        bool visible = false;
        CPlayerWeaponObject *held = currentWeapon;
        held->visible = visible;
        held->SetDrawDelay(1);
        currentWeapon->weapon->muzzleFlashStatus = visible;
        if (currentWeapon->weapon) currentWeapon->weapon->field_310 = 0.0f;
    }
    field_dc0 = 180.0f;
    flags.pitchWhileMounted = 0;
    BeginCrouch();
    if (!g_bInMultiplayerMode) CFader::FadeOut(CFader::BLACK, 180.0f);
    deathYaw = yaw;
    deathPitch = pitch;
}
void CPlayerObject::UpdateDeathSequence(float dt) {
    float time = field_dc0 * 0.00555555569f;
    float left = 1.0f - time;
    float yawStep = cosf(left * 7.85398197f) * (dt * 0.05f);
    float pitchStep = cosf(3.14159265f / (left + 0.5f)) * 0.035f / (left + left + 1.0f) * dt;
    field_dc0 -= dt;
    if (!g_bInMultiplayerMode) {
        yaw += yawStep;
        pitch -= pitchStep;
    } else {
        CMatrix look;
        if (!CMatrix::s_ClassInit) CMatrix::InitClass();
        CVector3 front = look.GetFront();
        float angle = atan2f(front.x, front.y);
        yaw = (angle - deathYaw) * left + deathYaw;
        pitch = (front.z * -3.14159265f - deathPitch) * left + deathPitch;
    }
    if (!(field_dc0 > 0.0f)) {
        field_dc0 = 0.0f;
        ((ISceneNode *)this)->TriggerScriptEvent(121, 0, false);
    } else if (!g_bInMultiplayerMode && !g_bCoopMultiplayer) {
        SoundSetMasterVolume((int)(time * 127.0f));
    }
}
