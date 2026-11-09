#include "SceneNode.h"
#include "DrawContext.h"
class CWorldObject {
  public:
    void DrawShadows(CDrawContext &);
};
struct SceneNodeLink {
    unsigned char unknown00[28];
    ISceneNode *next;
};
struct SceneDrawListLink {
    unsigned char unknown00[60];
    ISceneNode *next;
};
struct SceneDrawProjector {
    unsigned char unknown00[592];
    int enabled; // +0x250
};
inline bool operator!=(const CScene::NodeIterator &a, const CScene::NodeIterator &b) { return a.node != b.node; }
inline void Next(CScene::NodeIterator &it) { it.node = ((SceneNodeLink *)it.node)->next; }

void CScene::Draw(CDrawContext &context, CFog &sceneFog) {
    context.sceneCamera = camera;
    context.fog = &sceneFog;
    context.visibleLists[0] = &fast60;
    context.visibleLists[1] = &fast78;
    context.visibleLists[2] = &fast90;
    fast60.clear();
    fast78.clear();
    fast90.clear();
    ISceneNode *node;
    for (node = firstLight; node; node = ((SceneDrawListLink *)node)->next) {
        if (node->IsVisible(context))
            fast60.push_back(node);
    }
    for (node = firstProjector; node; node = ((SceneDrawListLink *)node)->next) {
        if (((SceneDrawProjector *)node)->enabled && node->IsVisible(context))
            fast90.push_back(node);
    }
    Lock();
    context.InitCommands();
    if (skyBox)
        ((ISceneNode *)skyBox)->Draw(context);
    for (NodeIterator it = GetNodesBegin(); it != GetNodesEnd(); Next(it)) {
        if (it.node->IsDrawEnabled() && it.node->IsVisible(context))
            it.node->Draw(context);
    }
    world->DrawShadows(context);
    context.ExecCommands();
    Unlock();
}
