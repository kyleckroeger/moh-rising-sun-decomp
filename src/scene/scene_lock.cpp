// CScene::Lock defers scene changes; Unlock runs the queued changes in order.
#include "SceneNode.h"

void CScene::Lock() {
    locked = 1;
}

void CScene::Unlock() {
    locked = 0;
    SModCommand *command = queue.items;
    while (command != queue.End()) {
        SModCommand current = *command++;
        (this->*current.function)(current);
    }
    queue.count = 0;
}
