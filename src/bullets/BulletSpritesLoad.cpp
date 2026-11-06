// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "CameraShake.h"
extern "C" int strcmp(const char *, const char *);
class CBulletFactory {
  public:
    ProjectileBulletProperties_struct *projectileProperties;
    ThrownBulletProperties_struct *thrownProperties;
    void LoadBulletGUISprites();
    void DeleteBulletGUISprites();
};
void CBulletFactory::LoadBulletGUISprites() {
    int i;
    for (i = 0; i < 36; i++) {
        projectileProperties[i].sprite = 0;
        const char *name = projectileProperties[i].name;
        if (name[0]) {
            for (int j = 0; j < i; j++) {
                if (!strcmp(name, projectileProperties[j].name)) {
                    projectileProperties[i].sprite = projectileProperties[j].sprite;
                    break;
                }
            }
            if (!projectileProperties[i].sprite) {
                projectileProperties[i].sprite = new ("source/weapons/bulletfactory.cpp:567") CSprite(name, CSprite::SpriteLoadUnknown1);
                CSprite *sprite = (CSprite *)projectileProperties[i].sprite;
                if (!sprite->loaded) {
                    delete sprite;
                    projectileProperties[i].sprite = 0;
                }
            }
        }
    }
    for (i = 0; i < 12; i++) {
        thrownProperties[i].sprite = 0;
        const char *name = thrownProperties[i].name;
        if (name[0]) {
            for (int j = 0; j < i; j++) {
                if (!strcmp(name, thrownProperties[j].name)) {
                    thrownProperties[i].sprite = thrownProperties[j].sprite;
                    break;
                }
            }
            if (!thrownProperties[i].sprite) thrownProperties[i].sprite = new ("source/weapons/bulletfactory.cpp:603") CSprite(name, CSprite::SpriteLoadUnknown1);
        }
    }
}
