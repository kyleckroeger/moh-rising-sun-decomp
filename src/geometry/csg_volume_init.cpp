// Fragment of csg_volume.cpp: construction, destruction (the key function, so the table is
// emitted here) and Init.
#include "CSGVolume.h"

CCSGVolume::CCSGVolume() : object(0), index(0) {}
CCSGVolume::~CCSGVolume() {}
void CCSGVolume::Init(int i, CHierObject *o) {
    index = i;
    object = o;
}
