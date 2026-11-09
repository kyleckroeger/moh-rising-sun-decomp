// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original pumhandler.cpp.
#include "Objectives.h"
#include "PopUpMessage.h"

extern unsigned char maxPopupsPerViewport;

unsigned short PopUpMessageHandler::GetOldest(unsigned char viewport) {
    unsigned short oldest = popups[viewport][0].Get_top();
    unsigned short index = 0;
    switch (viewport) {
    case 0:
    case 4:
        for (short i = 1; i < maxPopupsPerViewport; i++) {
            unsigned short top = popups[viewport][i].Get_top();
            if (top > oldest) {
                oldest = top;
                index = i;
            }
        }
        break;
    case 1:
    case 2:
        for (short i = 1; i < maxPopupsPerViewport; i++) {
            unsigned short top = popups[viewport][i].Get_top();
            if (top < oldest) {
                oldest = top;
                index = i;
            }
        }
        break;
    }
    popups[viewport][index].Reset();
    return index;
}

// The loop for viewports 1 and 2 reuses its counter in an inner loop, as the original does.
short PopUpMessageHandler::GetNewest(unsigned char viewport) {
    short result = -1;
    short i;
    short best;
    switch (viewport) {
    case 0:
    case 4:
        best = 2345;
        for (i = 0; i < maxPopupsPerViewport; i++) {
            if (!popups[viewport][i].Available()) {
                short top = popups[viewport][i].Get_top();
                if (top < best) {
                    best = top;
                    result = i;
                }
            }
        }
        break;
    case 1:
    case 2:
        best = 2345;
        for (i = 0; i < maxPopupsPerViewport; i++) {
            for (i = 1; i < maxPopupsPerViewport; i++) {
                short top = popups[viewport][i].Get_top();
                if (top < best) {
                    best = top;
                    result = i;
                }
            }
        }
        break;
    }
    return result;
}

short PopUpMessageHandler::CheckForEmptySlot(unsigned char viewport) {
    short i;
    short result;
    for (i = 0; i < maxPopupsPerViewport; i++) {
        if (popups[viewport][i].Available()) {
            result = i;
            goto done;
        }
    }
    if (viewport != 3)
        result = GetOldest(viewport);
    else
        result = -1;
done:
    return result;
}

void PopUpMessageHandler::UpdateScroll(unsigned char viewport, short id) {
    for (short i = 0; i < maxPopupsPerViewport; i++) {
        unsigned char property = popups[viewport][i].Get_property();
        if (!popups[viewport][i].Available() && popups[viewport][i].Get_viewportID() == id) {
            switch (property) {
            case 1:
                popups[viewport][i].Set_property(4);
                break;
            case 2:
                popups[viewport][i].Set_property(5);
                break;
            case 3:
                popups[viewport][i].Set_property(6);
                break;
            case 7:
                popups[viewport][i].Set_property(8);
                break;
            }
            popups[viewport][i].MoveTop();
        }
    }
}
