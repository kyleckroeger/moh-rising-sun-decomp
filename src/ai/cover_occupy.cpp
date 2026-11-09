// CCoverPoint: taking and releasing the point and marking it out of area.
#include "AIBehavior.h"
int CCoverPoint::FlagAsOccupied() {
    if (IsOccupied())
        return 0;
    occupied = 1;
    return 1;
}
void CCoverPoint::FlagAsUnoccupied() { occupied = 0; }
void CCoverPoint::SetOutOfArea(bool value) {
    if (value)
        outOfArea = 1;
    else
        outOfArea = 0;
}
