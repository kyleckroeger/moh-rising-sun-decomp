// AI-assisted reconstruction from GR8E69; see docs/Bullets.md.
#include "Bullet.h"
#include "Light.h"
#include "ParticleClock.h"
void CThrownBullet::CreateExplosionEffect(const CMatrix &matrix, bool extra) {
    if (explosionParticleSystems[0]) {
        IParticleSystem *system = CPSManager::Create(explosionParticleSystems[0], true, (eParticleSystem)0);
        if (system) system->SetTMLocalToWorld(matrix);
    }
    if (explosionParticleSystems[1]) {
        IParticleSystem *system = CPSManager::Create(explosionParticleSystems[1], true, (eParticleSystem)0);
        if (system) system->SetTMLocalToWorld(matrix);
    }
    if (explosionParticleSystems[2]) {
        IParticleSystem *system = CPSManager::Create(explosionParticleSystems[2], true, (eParticleSystem)0);
        if (system) system->SetTMLocalToWorld(matrix);
    }
    if (extra && explosionParticleSystems[3]) {
        IParticleSystem *system = CPSManager::Create(explosionParticleSystems[3], true, (eParticleSystem)0);
        if (system) system->SetTMLocalToWorld(matrix);
    }
    CLight *light = CLight::Create(0x9bce1511);
    if (light) {
        CVector3 position = matrix.GetPos();
        CVector3 &p = position;
        p.z += 0.5f;
        light->SetPosition(position);
    }
}
