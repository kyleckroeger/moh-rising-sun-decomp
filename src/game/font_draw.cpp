// CFont::DrawText snaps coordinates through the screen's 2D transform when asked and applies
// the justification before drawing.
#include "Font.h"
#include "Screen.h"
#include <math.h>

void CFont::DrawText(float x, float y, const char *text) {
    float drawX, drawY;
    if (snapX)
        drawX = floorf(x * CScreen::m_2dXScale + CScreen::m_2dXDisp + 0.5f);
    else
        drawX = x;
    if (snapY)
        drawY = floorf(y * CScreen::m_2dYScale + CScreen::m_2dYDisp + 0.5f);
    else
        drawY = y;
    if (justification)
        ComputeJustification(drawX, drawY, text);
    FONT_drawtexta(handle, drawX, drawY, (const unsigned char *)text);
}
