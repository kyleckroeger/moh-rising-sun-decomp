// CScene::ReOrder moves a node to the end of the node list.
#include "SceneNode.h"

void CScene::ReOrder(ISceneNode *, ISceneNode *node) {
    if (node) {
        Remove(*node);
        Add(*node, 0);
    }
}
