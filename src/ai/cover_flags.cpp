// CCoverPoint: clearing the state bits.
#include "AIBehavior.h"
void CCoverPoint::InitFlags() { *(unsigned int *)this &= 0xffff0000; }
