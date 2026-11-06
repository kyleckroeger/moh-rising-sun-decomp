// AI-assisted reconstruction from GR8E69; Frontline reference credited in docs/Bullets.md.
#include "Bullet.h"
#include "WorldObject.h"
class CAIObject;
void MakeNoise(void *, CAIObject *, const CVector3 &, int, float, bool, int);

CExplosion::CExplosion(CScene &scene, const CVector3 &position, float radius,
                      float power, float amount, EScriptBulletType type,
                      ISceneNode *origin, bool checkHeight, bool flag)
    : sphere(radius, position), strength(power), damage(amount), bulletType(type),
      source(origin), heightCheck(checkHeight), field_44(flag) {
    MakeNoise(0, 0, position, 2, power * 6.0f, true, 5);
    scene.VisitAllNodesInVolume(*this, sphere, (ISceneNode::EVolumeType)0, true);
    if (scene.world)
        scene.world->HandleExplosion(*this);
}
