// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "Light.h"
#include "Light.h"
#include "Light.h"
#include "Light.h"
#include "Light.h"
#include "Light.h"
unsigned int CThrownBullet::IsCollisionEnabled() const { return 1; }
CLight *CThrownBullet::GetAttachedLight() const { return light.operator->(); }
