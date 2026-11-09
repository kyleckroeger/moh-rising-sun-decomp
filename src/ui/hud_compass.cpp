// CHeadsUpDisplay::UpdateCompassRotation damps the compass, turns it toward the player's facing by the
// shortest way and keeps the angle within half a turn; GetCompassPosition places it on screen. Part of the
// original HeadsUpDisplay.cpp. Both arms of the velocity update are written out because the compiled code
// merges them.
#include "HeadsUpDisplay.h"
#include "Screen.h"
#include "CVector3.h"
extern "C" float floorf(float);
extern "C" float sqrtf(float);
extern "C" float acosf(float);
extern int g_bInMultiplayerMode;
struct ViewportRect {
    float left;
    float top;
    float right;
    float bottom;
};
extern ViewportRect g_viewport;
inline float Dot(const CHeadsUpDisplay::SRect &a, const CVector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline void Cross(CVector3 &out, const CVector3 &a, const CHeadsUpDisplay::SRect &b) {
    CVector3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    out.x = r.x;
    out.y = r.y;
    out.z = r.z;
}
void CHeadsUpDisplay::UpdateCompassRotation(CPlayerObject *player, float time) {
    if (!g_bInMultiplayerMode)
        for (int i = 0; i < time; i++)
            compassVelocity *= 0.95f;
    CVector3 forward;
    ((ISceneNode *)player)->GetForward(forward);
    float length = sqrtf(forward.LengthSquared());
    if (length != 0.0f)
        forward *= 1.0f / length;
    float angle = acosf(Dot(rect10, forward));
    if (unknown83c)
        angle += 3.1415927f;
    CVector3 cross;
    Cross(cross, forward, rect10);
    if (!(cross.x <= forward.x))
        angle = -angle;
    if (g_bInMultiplayerMode) {
        compassAngle = angle;
    } else {
        float difference = 0.0f;
        if (!(angle <= compassAngle)) {
            difference = angle - compassAngle;
            if (!(difference <= 3.1415927f))
                difference -= 6.2831855f;
        } else if (!(angle >= compassAngle)) {
            difference = angle - compassAngle;
            if (!(difference >= -3.1415927f))
                difference += 6.2831855f;
        }
        if (difference < 0.0f) {
            compassVelocity += difference;
            compassAngle += compassVelocity * time * 0.016683352f;
        } else if (!(difference <= 0.0f)) {
            compassVelocity += difference;
            compassAngle += compassVelocity * time * 0.016683352f;
        }
        float shown = compassAngle;
        float limit = 3.1415927f;
        float negLimit = -3.1415927f;
        if (!(shown <= limit)) {
            do
                shown -= 6.2831855f;
            while (shown > limit);
        }
        if (!(shown >= negLimit)) {
            do
                shown += 6.2831855f;
            while (shown < negLimit);
        }
        compassAngle = shown;
    }
}
void CHeadsUpDisplay::GetCompassPosition(float &x, float &y, float scale) {
    float left = g_viewport.left;
    float bottom = g_viewport.bottom;
    if (left == 0.0f)
        left = floorf(left * CScreen::m_2dXScale + CScreen::m_2dXDisp + 0.5f);
    if (bottom == CScreen::m_sHeight)
        bottom = floorf(bottom * CScreen::m_2dYScale + CScreen::m_2dYDisp + 0.5f);
    float half = scale * 0.5f;
    float offset = half * 96.0f;
    x = left + offset;
    y = bottom - offset;
}
