// GCHW_VD: the drawn rectangle's position and scale.
#include "VideoDriver.h"

void GCHW_VD::SetPosition(float newX, float newY, float newZ) {
    right = width * scaleX + newX;
    bottom = height * scaleY + newY;
    x = newX;
    y = newY;
    z = newZ;
}

void GCHW_VD::SetScale(float newX, float newY) {
    right = width * newX + x;
    scaleX = newX;
    scaleY = newY;
    bottom = height * newY + y;
}
