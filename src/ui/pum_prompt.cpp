// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original pumhandler.cpp. PromptUserMessage repeats a message's settings with new text.
#include "Objectives.h"
#include "PopUpMessage.h"

void PromptUserMessage(char *text, PopUpMessageHandler *handler, PopUpMessage *message) {
    unsigned char placement = message->Get_placement();
    unsigned short x = message->Get_screenLocX();
    unsigned short y = message->Get_screenLocY();
    unsigned char property = message->Get_property();
    float time = message->Get_onScreenTime();
    unsigned char r = message->Get_r();
    unsigned char g = message->Get_g();
    unsigned char b = message->Get_b();
    short viewport = message->Get_viewportID();
    short id = message->Get_messageID();
    handler->AddPopUpMessage(text, placement, x, y, property, time, 22, r, g, b, 70, viewport, id);
}
