// CScene: changes to the scene, applied now or queued while the scene is locked.
#include "SceneNode.h"

void CScene::AddPlayer(CPlayerObject *player) {
    SModCommand command;
    command.function = &CScene::AddPlayer_now;
    command.arg0 = player;
    command.arg1 = 0;
    if (locked)
        queue.items[queue.count++] = command;
    else
        AddPlayer_now(command);
}

void CScene::SetWorld(CWorldObject *newWorld) {
    SModCommand command;
    command.function = &CScene::SetWorld_now;
    command.arg0 = newWorld;
    command.arg1 = 0;
    if (locked)
        queue.items[queue.count++] = command;
    else
        SetWorld_now(command);
}

void CScene::SetCamera(CCamera *camera) {
    SModCommand command;
    command.function = &CScene::SetCamera_now;
    command.arg0 = camera;
    command.arg1 = 0;
    if (locked)
        queue.items[queue.count++] = command;
    else
        SetCamera_now(command);
}

void CScene::SetSkyBox(CSkyBox *skyBox) {
    SModCommand command;
    command.function = &CScene::SetSkyBox_now;
    command.arg0 = skyBox;
    command.arg1 = 0;
    if (locked)
        queue.items[queue.count++] = command;
    else
        SetSkyBox_now(command);
}

void CScene::Add(ISceneNode &node, ISceneNode *) {
    SModCommand command;
    command.function = &CScene::AddNode_now;
    command.arg0 = &node;
    command.arg1 = 0;
    if (locked)
        queue.items[queue.count++] = command;
    else
        AddNode_now(command);
}

void CScene::Remove(ISceneNode &node) {
    SModCommand command;
    command.function = &CScene::RemoveNode_now;
    command.arg0 = &node;
    command.arg1 = 0;
    if (locked)
        queue.items[queue.count++] = command;
    else
        RemoveNode_now(command);
}
