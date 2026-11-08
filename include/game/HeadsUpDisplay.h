#ifndef GAME_HEADSUPDISPLAY_H
#define GAME_HEADSUPDISPLAY_H
// A scoped view of the heads-up display for its small state functions. Names of the class, methods
// and the InterfaceState enum come from the symbol table; member names are inferred from use.
#include "Color.h"
#include "SceneNode.h"
class CSprite;
enum InterfaceState { InterfaceStateUnknown = 0, InterfaceStateOn = 1 };

class CFont;
class CHeadsUpDisplay {
public:
    CHeadsUpDisplay();
    ~CHeadsUpDisplay();
    void Shutdown();
    void Reset();
    void UpdateHitStatus(float elapsed);
    void ChangeHitStatus(unsigned int direction);
    void SetCompassState(InterfaceState state);
    void SetAmmoState(InterfaceState state);
    void AddHitDirection(unsigned int direction);
    void SetTarget(const char *name);
    void DrawViewportEdge();
    void DrawSprite(float, float, float, float, CSprite &, CColor, bool, float);
    static CSprite *healthSprite;
    static CSprite *compassSprite;
    static CSprite *compassEdgeSprite;
    static CSprite *hitMeterSprite;
    static CSprite *blackSprite;
    static CSprite *dotSprite;
    static CSprite *greySprite;
    static CSprite *starSprite;
    static CSprite *circleSprite;
    static CFont *bulletText;
    static int bulletTextCount;

    struct SRect {
        SRect(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
        float x, y, z, w;
    };
    unsigned char unknown000[0x10];
    SRect rect10;                // +0x10
    unsigned char unknown020[0x7ac - 0x20];
    unsigned int state7ac : 4;   // +0x7ac
    unsigned int unknown7ac : 28;
    float hitStatus[4];          // +0x7b0
    float unknown7c0;
    float unknown7c4;            // +0x7c4
    float unknown7c8;
    float unknown7cc;
    float unknown7d0;
    float unknown7d4;
    float unknown7d8;
    InterfaceState compassState; // +0x7dc
    InterfaceState ammoState;    // +0x7e0
    float compassTime;           // +0x7e4
    float ammoTime;              // +0x7e8
    float unknown7ec;            // +0x7ec
    float targetTime;            // +0x7f0
    char targetName[11];         // +0x7f4
    unsigned char unknown7ff[0x804 - 0x7ff];
    float unknown804;            // +0x804
    int unknown808;              // +0x808
    float unknown80c;            // +0x80c
    unsigned char unknown810[0x818 - 0x810];
    int unknown818;              // +0x818
    unsigned char unknown81c[0x828 - 0x81c];
    int unknown828;
    int unknown82c;
    int unknown830;
    int unknown834;
    float unknown838;
    int unknown83c;
    IMovingSceneNode *player;    // +0x840
};
#endif
