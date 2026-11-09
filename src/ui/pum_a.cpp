// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original pumhandler.cpp.
#include "Objectives.h"
#include "Font.h"

class CSprite {
public:
    ~CSprite();
};

void PopUpMessageHandler::LoadSubtitles(char *a, char *b, char *c) {
    tables[1].Load(a, b, c);
}

void PopUpMessageHandler::ShutdownPopUpMessageHandler() {
    delete font648;
    font648 = 0;
    delete sprite650;
    sprite650 = 0;
    delete sprite64c;
    sprite64c = 0;
    delete font644;
    font644 = 0;
}
