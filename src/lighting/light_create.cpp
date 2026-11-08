// CLight's creation entry points, which forward to the animated-light manager.
#include "Light.h"

extern CAnimLightManager g_AnimLightManager;

CLight *CLight::Create(unsigned int id) {
    return g_AnimLightManager.Create(id);
}

CLight *CLight::Create(void *properties) {
    return g_AnimLightManager.Create((MOH_animatedLight_Struct *)properties);
}
