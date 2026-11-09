// AI-assisted reconstruction from GR8E69. Part of the original HeadsUpDisplay.cpp.
#include "HeadsUpDisplay.h"

void CHeadsUpDisplay::UpdateHitStatus(float elapsed) {
    float step = elapsed * 0.0166833512f;
    for (int i = 0; i < 4; i++) {
        if (!(hitStatus[i] <= 0.0f)) {
            hitStatus[i] -= step;
            if (!(hitStatus[i] >= 0.0f))
                hitStatus[i] = 0.0f;
        }
    }
}

void CHeadsUpDisplay::ChangeHitStatus(unsigned int direction) {
    direction--;
    if (direction <= 3)
        hitStatus[direction] = 3.0f;
}
