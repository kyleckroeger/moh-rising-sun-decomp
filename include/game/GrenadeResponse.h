// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#ifndef GAME_GRENADE_RESPONSE_H
#define GAME_GRENADE_RESPONSE_H

#include "CVector3.h"
class CSoldierObject;
class CThrownBullet;

// Original free-function names and argument types; ordinary return types are
// inferred from their bodies and callers. No behavior-object layout is implied.
CVector3 GetVectorToGrenade(CSoldierObject *, const CThrownBullet *);
bool IsAlive(CSoldierObject *);
bool IsCatchable(CSoldierObject *, CThrownBullet *, const CVector3 &, float);
bool IsStoppedOnGround(CSoldierObject *, const CThrownBullet *);
bool IsStoppedOnGround(const CVector3 &, float);
float DistanceSquaredToGrenade(CSoldierObject *, CThrownBullet *);
bool InDanger(CSoldierObject *, CThrownBullet *);

#endif
