// CScene::AddNodeExtents enters a node's begin and end extents on both sweep lists; RemoveNodeExtents
// takes them off again and clears the node's links to them.
#include "SceneNode.h"
struct SceneNodeExtents {
    unsigned char unknown00[32];
    ISceneNode::SExtent *extents[4];
};
void CScene::AddNodeExtents(ISceneNode &node) {
    SceneNodeExtents &links = (SceneNodeExtents &)node;
    ISceneNode::SExtent extent;
    extent.node = &node;
    extent.isBegin = 1;
    links.extents[0] = &*extents0.insert(extents0.end(), extent);
    extent.isBegin = 0;
    links.extents[1] = &*extents0.insert(extents0.end(), extent);
    extent.isBegin = 1;
    links.extents[2] = &*extents1.insert(extents1.end(), extent);
    extent.isBegin = 0;
    links.extents[3] = &*extents1.insert(extents1.end(), extent);
}
inline bool operator==(const ISceneNode::SExtent &a, const ISceneNode::SExtent &b) {
    return a.node == b.node && a.isBegin == b.isBegin;
}
template <class I, class T> inline I Find(I first, I last, const T &value) {
    I i = first;
    for (; i != last; ++i)
        if (*i == value)
            break;
    return i;
}
void CScene::RemoveNodeExtents(ISceneNode &node) {
    SceneNodeExtents &links = (SceneNodeExtents &)node;
    dwi::list<ISceneNode::SExtent>::iterator it(0);
    ISceneNode::SExtent key;
    key.node = &node;
    key.isBegin = 1;
    it = Find(extents0.begin(), extents0.end(), key);
    if (it != extents0.end())
        extents0.erase(it);
    links.extents[0] = 0;
    key.isBegin = 0;
    it = Find(extents0.begin(), extents0.end(), key);
    if (it != extents0.end())
        extents0.erase(it);
    links.extents[1] = 0;
    key.isBegin = 1;
    it = Find(extents1.begin(), extents1.end(), key);
    if (it != extents1.end())
        extents1.erase(it);
    links.extents[2] = 0;
    key.isBegin = 0;
    it = Find(extents1.begin(), extents1.end(), key);
    if (it != extents1.end())
        extents1.erase(it);
    links.extents[3] = 0;
}
