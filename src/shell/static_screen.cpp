// AI-assisted reconstruction from GR8E69. Part of the original StaticScreen.cpp. The screen owns two
// loading-screen sprites and a small state block. Member names are inferred from these functions.
#include "CameraShake.h"

class CScreen;
extern "C" void *DWI_alloc(const char *, int, int);

// The 16-byte block Init allocates; only the word it clears is established.
struct StaticScreenState {
    int unknown00[3];
    int unknown0c; // +0x0c
};

class CStaticScreen {
  public:
    CStaticScreen();
    virtual ~CStaticScreen();
    void Init(CScreen *);

    CScreen *screen;        // +0x00
    CSprite *sprite04;      // +0x04
    CSprite *sprite08;      // +0x08
    int unknown0c;          // +0x0c
    char name[64];          // +0x10, the loading screen's sprite name
    StaticScreenState *state; // +0x50
    int unknown54;          // +0x54
};

CStaticScreen::CStaticScreen() {
    screen = 0;
    sprite04 = 0;
    sprite08 = 0;
    unknown0c = 0;
    state = 0;
    unknown54 = 0;
}

CStaticScreen::~CStaticScreen() {
    if (sprite08)
        delete sprite08;
    if (sprite04)
        delete sprite04;
    if (state)
        delete state;
}

void CStaticScreen::Init(CScreen *newScreen) {
    StaticScreenState *current = state;
    screen = newScreen;
    if (!current) {
        state = (StaticScreenState *)DWI_alloc("source/shell/StaticScreen.cpp:109", 16, 1024);
        state->unknown0c = 0;
        name[0] = 0;
    }
}
