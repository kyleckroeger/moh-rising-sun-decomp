// CCamera::AttemptUpdate refreshes stale matrices, copies position components to four followers
// and publishes the world-to-camera matrix and the position to their globals.
#include "Camera.h"

extern CVector3 g_CameraPosition;
extern CMatrix g_WorldToCamera;

void CCamera::AttemptUpdate(float) {
    if (!worldToCameraValid)
        UpdateWorldToCamera();
    if (!cameraToClipValid)
        UpdateCameraToClip();
    if (!worldToClipValid)
        UpdateWorldToClip();
    CVector3 position;
    GetPosition(position);
    if (followers[0])
        followers[0]->value = position.x;
    if (followers[2])
        followers[2]->value = position.y;
    if (followers[1])
        followers[1]->value = position.x;
    if (followers[3])
        followers[3]->value = position.y;
    g_WorldToCamera = worldToCamera;
    g_CameraPosition = position;
}
