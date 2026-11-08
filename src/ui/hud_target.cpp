// AI-assisted reconstruction from GR8E69. Part of the original HeadsUpDisplay.cpp.
#include "HeadsUpDisplay.h"
#include <string.h>

void CHeadsUpDisplay::SetTarget(const char *name) {
    if (strlen(name) <= 10) {
        strcpy(targetName, name);
    } else {
        strncpy(targetName, name, 10);
        targetName[10] = 0;
    }
    targetTime = 1.0f;
}

// Eight vertices of 24 bytes; the fourth word of each is the screen height coordinate.
struct LetterBoxVert {
    float unknown00[3];
    float y; // +0x0c
    float unknown10[2];
};
extern LetterBoxVert LetterBoxVerts[8];

void ChangeLetterboxWidth(float amount) {
    LetterBoxVerts[2].y -= amount;
    LetterBoxVerts[3].y -= amount;
    LetterBoxVerts[4].y += amount;
    LetterBoxVerts[5].y += amount;
}
