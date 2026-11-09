// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original HeadsUpDisplay.cpp. DrawViewportEdge lets the first player draw the dividing
// lines of a split screen in half-intensity grey.
#include "HeadsUpDisplay.h"
#include "Screen.h"

extern int g_NumPlayers;

void CHeadsUpDisplay::DrawViewportEdge() {
    if (g_NumPlayers != 1 && player->GetPlayerIndex() == 0) {
        float width = (float)CScreen::m_sWidth;
        float height = (float)CScreen::m_sHeight;
        if (g_NumPlayers == 2) {
            DrawSprite(0.0f, height * 0.5f - 1.0f, width, 2.0f, *blackSprite, CColor(128, 128, 128, 128), false, 0.0f);
        } else {
            DrawSprite(0.0f, height * 0.5f - 1.0f, width, 2.0f, *blackSprite, CColor(128, 128, 128, 128), false, 0.0f);
            DrawSprite(width * 0.5f - 1.0f, 0.0f, 2.0f, height, *blackSprite, CColor(128, 128, 128, 128), false, 0.0f);
        }
    }
}
