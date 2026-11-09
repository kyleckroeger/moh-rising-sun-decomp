// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original pumhandler.cpp.
#include "Objectives.h"
#include "PopUpMessage.h"

extern unsigned char maxPopupsPerViewport;

void PopUpMessageHandler::Clear() {
    for (int viewport = 0; viewport <= 4; viewport++) {
        for (int i = 0; i < maxPopupsPerViewport; i++) {
            if (!popups[viewport][i].Available())
                popups[viewport][i].Reset();
        }
    }
}
