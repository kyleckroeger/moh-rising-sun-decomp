// CScene: changes to the scene, applied now or queued while the scene is locked.
#include "SceneNode.h"

void CScene::Shutdown() {
    while (firstNode)
        Remove(*firstNode);
    Reset();
}

