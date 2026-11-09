// AI-assisted reconstruction from GR8E69. Part of the original HeadsUpDisplay.cpp.
#include "HeadsUpDisplay.h"

void CHeadsUpDisplay::SetCompassState(InterfaceState state) {
    if (state == InterfaceStateOn)
        compassTime = 5.0f;
    else
        compassTime = 4.5f;
    compassState = state;
}

void CHeadsUpDisplay::SetAmmoState(InterfaceState state) {
    ammoState = state;
    ammoTime = 4.5f;
}
