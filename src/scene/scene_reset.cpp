// CScene::Reset removes every node, clears the player, list and count members, and empties the extent
// lists, vectors and queues.
#include "SceneNode.h"
#include "SceneCollisions.h"
void CScene::Reset() {
    while (firstNode)
        Remove(*firstNode);
    playerCount = 0;
    int count = 4;
    int i;
    for (i = count - 1; i >= 0; i--)
        players[i] = 0;
    firstNode = 0;
    lastNode = 0;
    world = 0;
    camera = 0;
    firstStaticObject = 0;
    lastStaticObject = 0;
    firstAnimObject = 0;
    lastAnimObject = 0;
    firstLight = 0;
    lastLight = 0;
    firstProjector = 0;
    lastProjector = 0;
    nodeCount = 0;
    staticObjectCount = 0;
    animObjectCount = 0;
    lightCount = 0;
    projectorCount = 0;
    fast60.clear();
    fast90.clear();
    fast78.clear();
    extents0.clear();
    extents1.clear();
    pairs0.clear();
    pairs1.clear();
    collisions.Clear();
    exclusions.Clear();
    queue.Clear();
}
