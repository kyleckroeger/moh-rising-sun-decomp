#ifndef GAME_HEADSUPDISPLAY_H
#define GAME_HEADSUPDISPLAY_H
// A scoped view of the heads-up display for its small state functions. Names of the class, methods
// and the InterfaceState enum come from the symbol table; member names are inferred from use.
#include "Color.h"
#include "SceneNode.h"
class CSprite;
enum InterfaceState { InterfaceStateUnknown = 0, InterfaceStateOn = 1 };

class CHeadsUpDisplay {
public:
    void Reset();
    void UpdateHitStatus(float elapsed);
    void ChangeHitStatus(unsigned int direction);
    void SetCompassState(InterfaceState state);
    void SetAmmoState(InterfaceState state);
    void AddHitDirection(unsigned int direction);
    void SetTarget(const char *name);
    void DrawViewportEdge();
    void DrawSprite(float, float, float, float, CSprite &, CColor, bool, float);
    static CSprite *blackSprite;

    unsigned char unknown000[0x7b0];
    float hitStatus[4];          // +0x7b0
    unsigned char unknown7c0[0x7dc - 0x7c0];
    InterfaceState compassState; // +0x7dc
    InterfaceState ammoState;    // +0x7e0
    float compassTime;           // +0x7e4
    float ammoTime;              // +0x7e8
    unsigned char unknown7ec[0x7f0 - 0x7ec];
    float targetTime;            // +0x7f0
    char targetName[11];         // +0x7f4
    unsigned char unknown7ff[0x818 - 0x7ff];
    int unknown818;              // +0x818
    unsigned char unknown81c[0x840 - 0x81c];
    IMovingSceneNode *player;    // +0x840
};
#endif
