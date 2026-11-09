// Message accessors, the top line's movement, scrolling and the display timer.
// The constructor and Reset before them are not part of this unit.
#include <stddef.h>
extern "C" char *strncpy(char *, const char *, size_t);
#include "PopUpMessage.h"

void PopUpMessage::Set_placement(unsigned char value) { placement = value; }
void PopUpMessage::Set_screenLocs(unsigned short x, unsigned short y) { screenLocX = x; screenLocY = y; }
void PopUpMessage::Set_property(unsigned char value) { property = value; }
void PopUpMessage::Set_onScreenTime(float value) { onScreenTime = value; }
void PopUpMessage::Set_textSize(unsigned char value) { textSize = value; }
void PopUpMessage::Set_viewportID(short value) { viewportID = value; }
void PopUpMessage::Set_messageID(short value) { messageID = value; }
void PopUpMessage::Set_rgba(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
    r = red; g = green; b = blue; a = alpha;
}
void PopUpMessage::Set_top(unsigned short value) { top = value; }
void PopUpMessage::Set_bottom(unsigned short value) { bottom = value; }
void PopUpMessage::ChangeAvailability(unsigned char value) { available = value; }
void PopUpMessage::SetString(const char *text) { strncpy(string, text, 48); }
unsigned char PopUpMessage::Available() { return available; }
unsigned char PopUpMessage::Get_placement() { return placement; }
unsigned short PopUpMessage::Get_screenLocX() { return screenLocX; }
unsigned short PopUpMessage::Get_screenLocY() { return screenLocY; }
unsigned char PopUpMessage::Get_property() { return property; }
float PopUpMessage::Get_onScreenTime() { return onScreenTime; }
short PopUpMessage::Get_viewportID() { return viewportID; }
short PopUpMessage::Get_messageID() { return messageID; }
unsigned char PopUpMessage::Get_r() { return r; }
unsigned char PopUpMessage::Get_g() { return g; }
unsigned char PopUpMessage::Get_b() { return b; }
unsigned short PopUpMessage::Get_top() { return top; }
char *PopUpMessage::GetString() { return string; }

void PopUpMessage::MoveTop() {
    switch (placement) {
    case 0:
        top += textSize;
        break;
    case 1:
        top -= textSize;
        break;
    case 2:
        top -= textSize;
        break;
    case 4:
        top += textSize;
        break;
    }
}

void PopUpMessage::Scroll() {
    if (!(onScreenTime <= 0.0f)) {
        switch (placement) {
        case 0:
        case 4:
            screenLocY = top;
            switch (property) {
            case 4: property = 1; break;
            case 5: property = 2; break;
            case 6: property = 3; break;
            case 8: property = 7; break;
            }
            break;
        case 1:
        case 2:
            screenLocY = top;
            switch (property) {
            case 4: property = 1; break;
            case 5: property = 2; break;
            case 6: property = 3; break;
            case 8: property = 7; break;
            }
            break;
        }
    } else {
        Reset();
    }
}

void PopUpMessage::UpdateDisplayTime(float elapsed) { onScreenTime -= elapsed; }
