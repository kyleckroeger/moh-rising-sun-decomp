// CHeadsUpDisplay's destructor and Shutdown release the display's shared sprites and fonts. Part of the
// original HeadsUpDisplay.cpp.
#include "HeadsUpDisplay.h"
#include "Font.h"
class CSprite {
  public:
    ~CSprite();
};
extern CSprite *g_pAlliedIcon, *g_pJapaneseIcon, *g_pSmallAlliedIcon, *g_pSmallJapaneseIcon;
extern int bReady;
extern CFont *timestampFont;
extern int g_bInMultiplayerMode;
void ResetTimeStampStuff();

CHeadsUpDisplay::~CHeadsUpDisplay() {
    if (!g_bInMultiplayerMode) {
        delete healthSprite;
        delete compassSprite;
        delete hitMeterSprite;
        delete compassEdgeSprite;
        delete blackSprite;
        bulletTextCount--;
        if (bulletTextCount == 0) {
            delete bulletText;
            bulletText = 0;
        }
    }
}

void CHeadsUpDisplay::Shutdown() {
    bReady = 0;
    if (circleSprite) {
        delete circleSprite;
        circleSprite = 0;
    }
    if (g_pAlliedIcon) {
        delete g_pAlliedIcon;
        g_pAlliedIcon = 0;
    }
    if (g_pJapaneseIcon) {
        delete g_pJapaneseIcon;
        g_pJapaneseIcon = 0;
    }
    if (g_pSmallAlliedIcon) {
        delete g_pSmallAlliedIcon;
        g_pSmallAlliedIcon = 0;
    }
    if (g_pSmallJapaneseIcon) {
        delete g_pSmallJapaneseIcon;
        g_pSmallJapaneseIcon = 0;
    }
    if (compassSprite) {
        delete compassSprite;
        compassSprite = 0;
    }
    if (compassEdgeSprite) {
        delete compassEdgeSprite;
        compassEdgeSprite = 0;
    }
    if (healthSprite) {
        delete healthSprite;
        healthSprite = 0;
    }
    if (hitMeterSprite) {
        delete hitMeterSprite;
        hitMeterSprite = 0;
    }
    if (dotSprite) {
        delete dotSprite;
        dotSprite = 0;
    }
    if (greySprite) {
        delete greySprite;
        greySprite = 0;
    }
    if (starSprite) {
        delete starSprite;
        starSprite = 0;
    }
    if (blackSprite) {
        delete blackSprite;
        blackSprite = 0;
    }
    if (timestampFont) {
        delete timestampFont;
        timestampFont = 0;
    }
    ResetTimeStampStuff();
}
