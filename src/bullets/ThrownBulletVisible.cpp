// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "Fiber.h"
int CThrownBullet::IsVisible(CDrawContext &context) const { return ((const CVolFiber *)worldVolume)->CVolFiber::TestVisibility(context); }
