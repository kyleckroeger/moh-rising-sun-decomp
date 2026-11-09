// CCoverPoint: state queries.
#include "AIBehavior.h"
int CCoverPoint::IsOccupied() const { return occupied; }
int CCoverPoint::IsValid() const { return !invalid; }
int CCoverPoint::IsAvailable() const { return *((const unsigned short *)this + 1) == 0; }
