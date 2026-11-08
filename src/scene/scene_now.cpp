// CScene: applying a camera or sky box change.
#include "SceneNode.h"

void CScene::SetCamera_now(SModCommand &command) {
    camera = (CCamera *)command.arg0;
}

void CScene::SetSkyBox_now(SModCommand &command) {
    CSkyBox *newSkyBox = (CSkyBox *)command.arg0;
    if (newSkyBox != skyBox)
        skyBox = newSkyBox;
}
