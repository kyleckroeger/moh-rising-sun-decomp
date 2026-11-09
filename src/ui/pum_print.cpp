// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original pumhandler.cpp. PrintSubtitle queues subtitle text at a fixed spot.
#include "Objectives.h"
#include "Screen.h"
#include "Color.h"

void PrintSubtitle(char *text, float time, CColor color, PopUpMessageHandler *handler) {
    unsigned short x = (unsigned short)(int)(CScreen::m_sWidthScale * 93.0);
    unsigned short y = (unsigned short)(int)(CScreen::m_sHeightScale * 401.0);
    handler->AddPopUpMessage(text, 1, x, y, 1, time, 22, color.r, color.g, color.b, color.a, -1, -1);
}
