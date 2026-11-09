// CScene: the player list and the node list's iterators.
#include "SceneNode.h"

// Member-only view of a node's list links.
struct SceneNodeLinks {
    unsigned char unknown00[24];
    ISceneNode *previous;
    ISceneNode *next;
};

CPlayerObject *CScene::GetPlayer(int index) const {
    return players[index];
}

CScene::NodeIterator CScene::GetNodesBegin() const {
    return NodeIterator(firstNode);
}

CScene::NodeIterator CScene::GetNodesEnd() const {
    return NodeIterator(0);
}

int CScene::IsNodeInScene(const ISceneNode *node) const {
    if (!node)
        return 0;
    if (firstNode == node)
        return 1;
    if (lastNode == node)
        return 1;
    const SceneNodeLinks *links = (const SceneNodeLinks *)node;
    if (links->next)
        return 1;
    if (links->previous)
        return 1;
    return 0;
}
