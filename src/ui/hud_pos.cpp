// CHeadsUpDisplay::GetBulletStatsRightBottom gives the corner the ammo display hangs from, and DrawWeaponName
// draws the current weapon's name beside it. Part of the original HeadsUpDisplay.cpp. The same scale call on
// both paths of the split-screen test is inferred: the binary performs that test and makes the same call.
#include "HeadsUpDisplay.h"
#include "Screen.h"
#include "Font.h"
#include "Objectives.h"
extern int g_bInMultiplayerMode;
extern int g_bCoopMultiplayer;
extern "C" float floorf(float);
struct ViewportRect {
    float left;
    float top;
    float right;
    float bottom;
    float unknown10;
    float height; // +0x14
};
extern ViewportRect g_viewport;
void CHeadsUpDisplay::GetBulletStatsRightBottom(float &right, float &bottom) {
    right = g_viewport.right;
    bottom = g_viewport.bottom;
    if (right == (int)CScreen::m_sWidth)
        right = floorf(right * CScreen::m_2dXScale + CScreen::m_2dXDisp + 0.5f);
    if (bottom == CScreen::m_sHeight)
        bottom = floorf(bottom * CScreen::m_2dYScale + CScreen::m_2dYDisp + 0.5f);
}

void CHeadsUpDisplay::DrawWeaponName(const char *name) {
    if (name[0]) {
        CFont *font = g_pPopUpMessageHandler->font648;
        float right, bottom;
        GetBulletStatsRightBottom(right, bottom);
        CColor oldColor = font->GetColor();
        float alpha = unknown7c8 * 255.0f;
        font->SetColor(CColor(255, 255, 255, (int)(unknown7d8 * alpha)));
        float scale = 1.0f;
        float aspect = g_viewport.height / CScreen::m_sHeight;
        float scaleX, scaleY;
        font->GetScale(scaleX, scaleY);
        if (g_bInMultiplayerMode || g_bCoopMultiplayer)
            font->SetScale(CScreen::m_sWidthScale, CScreen::m_sHeightScale);
        else
            font->SetScale(CScreen::m_sWidthScale, CScreen::m_sHeightScale);
        float x, y, w, h;
        FONT_getrecta(font->handle, (const unsigned char *)name, &x, &y, &w, &h);
        if (!g_bInMultiplayerMode && !g_bCoopMultiplayer) {
            x = (int)CScreen::m_sWidth - w - CScreen::m_sWidthScale * 56.0f;
            font->DrawText(x, CScreen::m_sHeightScale * 356.0f, name);
        } else if (g_bInMultiplayerMode) {
            font->GetRenderedSize(name, w, h);
            x = right - 8.0f;
            y = bottom - h - scale * 48.0f - 2.0f;
            unsigned int justification = font->GetJustification();
            font->SetJustification(1);
            font->snapY = 0;
            font->snapX = 0;
            font->DrawText(x, y, name);
            font->SetJustification(justification);
        } else {
            x = g_viewport.right - w - CScreen::m_sWidthScale * 56.0f;
            float verticalOffset = aspect * 356.0f;
            y = CScreen::m_sHeightScale * verticalOffset + g_viewport.top - 32.0f;
            font->DrawText(x, y, name);
        }
        font->SetScale(scaleX, scaleY);
        font->SetColor(oldColor);
    }
}
