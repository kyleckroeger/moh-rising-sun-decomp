// CCamera's class setup: the loader variables for the camera position and world-to-camera matrix.
#include "Camera.h"
#include "EAGLDynamicLoader.h"

extern CVector3 g_CameraPosition;
extern CMatrix g_WorldToCamera;
extern int g_bReady;

void CCamera::InitClass() {
    EAGL::DynamicLoader::RegisterVar("Camera::CameraPosition", &g_CameraPosition);
    EAGL::DynamicLoader::RegisterVar("Camera::WorldToCamera", &g_WorldToCamera);
    g_bReady = 1;
}

void CCamera::ShutdownClass() {
    EAGL::DynamicLoader::UnRegisterVar("Camera::CameraPosition");
    EAGL::DynamicLoader::UnRegisterVar("Camera::WorldToCamera");
    g_bReady = 0;
}
