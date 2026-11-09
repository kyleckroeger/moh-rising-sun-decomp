// CLight's class setup.
#include "Light.h"
#include "EAGLDynamicLoader.h"

extern CAnimLightManager g_AnimLightManager;
extern unsigned char g_MOHLightBlock[];
extern int g_bReady;

void CLight::InitClass(CScene *scene) {
    g_AnimLightManager.InitClass(scene);
    EAGL::DynamicLoader::RegisterVar("g_pMOHLightBlock", g_MOHLightBlock);
    g_bReady = 1;
}

void CLight::ShutdownClass() {
    g_AnimLightManager.ShutdownClass();
    EAGL::DynamicLoader::UnRegisterVar("g_pMOHLightBlock");
    g_bReady = 0;
}

void CLight::ResetClass() {
    g_AnimLightManager.ResetClass();
}
