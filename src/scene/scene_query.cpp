// CScene: the player list and the node list's iterators.
#include "SceneNode.h"

CPlayerObject *CScene::GetPlayer(int index) const {
    return players[index];
}

CScene::NodeIterator CScene::GetNodesBegin() const {
    return NodeIterator(firstNode);
}

CScene::NodeIterator CScene::GetNodesEnd() const {
    return NodeIterator(0);
}
