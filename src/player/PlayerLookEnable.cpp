// AI-assisted reconstruction from GR8E69; see docs/PlayerObject.md.
#include "CameraShake.h"
#include "Camera.h"
extern CCamera g_camera;
void CPlayerObject::LookAtTarget::Enable(ISceneNode *node, float duration) {
    if (node) {
        link.Unlink();
        subject = node;
        node->AddObserver((IObserver *)this);
        time = duration;
        active = 1;
    } else {
        active = 0;
    }
}
void CPlayerObject::LookAtTarget::Enable(const CVector3 &position, float duration) {
    int zero = 0;
    link.Unlink();
    subject = (ISubject *)zero;
    time = duration;
    target = position;
    active = 1;
}
