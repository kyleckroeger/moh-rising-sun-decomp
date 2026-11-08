// AI-assisted reconstruction from GR8E69. Part of the original StaticScreen.cpp. StopLoading frees the
// two loading-screen sprites.
#include "CameraShake.h"

class CStaticScreen {
  public:
    virtual ~CStaticScreen();
    void StopLoading();

    int unknown00;
    CSprite *sprite04; // +0x04
    CSprite *sprite08; // +0x08
};

void CStaticScreen::StopLoading() {
    if (sprite04) {
        delete sprite04;
        sprite04 = 0;
    }
    if (sprite08) {
        delete sprite08;
        sprite08 = 0;
    }
}
