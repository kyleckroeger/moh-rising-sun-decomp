// AI-assisted reconstruction from GR8E69. Part of the original StaticScreen.cpp. Each function draws
// the loading screen twice with clearing off.
#include "CameraShake.h"
#include "Screen.h"

class CStaticScreen {
  public:
    virtual ~CStaticScreen();
    void DrawScreensFull();
    void DrawScreens();
    void DrawLoadingScreenFull(CSprite &);
    void DrawLoadingScreen(CSprite &);

    CScreen *screen;  // +0x00
    int unknown04[3];
    char name[64];    // +0x10, the loading screen's sprite name
};

void CStaticScreen::DrawScreens() {
    CSprite sprite(name, (CSprite::ESpriteLoadType)2);
    screen->SetClear(false);
    DrawLoadingScreen(sprite);
    DrawLoadingScreen(sprite);
}
