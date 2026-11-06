// AI-assisted reconstruction from GR8E69; see docs/SceneNode.md.
#include "SceneNode.h"
#include "Volume.h"

// Member-only views of the three intrusive links. Their offsets are established
// by the original traversals; these are not complete node allocations.
struct StaticObjectLinkView {
    unsigned char unknown_00[0x48];
    ISceneNode *next;
};
struct LightLinkView {
    unsigned char unknown_00[0x3c];
    ISceneNode *next;
};
struct SceneNodeLinkView {
    unsigned char unknown_00[0x1c];
    ISceneNode *next;
};

bool CScene::VisitAllStaticObjects(dwi::IVisitor<ISceneNode> &visitor) {
    for (ISceneNode *node = firstStaticObject; node;
         node = reinterpret_cast<StaticObjectLinkView *>(node)->next) {
        if (visitor.Visit(*node))
            return true;
    }
    return false;
}

bool CScene::VisitAllLights(dwi::IVisitor<ISceneNode> &visitor) {
    for (ISceneNode *node = firstLight; node;
         node = reinterpret_cast<LightLinkView *>(node)->next) {
        if (visitor.Visit(*node))
            return true;
    }
    return false;
}

bool CScene::VisitAllNodesInVolume(dwi::IVisitor<ISceneNode> &visitor,
                                 const IVolume &volume,
                                 ISceneNode::EVolumeType type, bool skipWorld) {
    ISceneNode *node = firstNode;
    for (; node; node = reinterpret_cast<SceneNodeLinkView *>(node)->next) {
        if (skipWorld && node->AsWorldObject())
            continue;
        CCollision collision;
        const IVolume *other = (const IVolume *)node->GetWorldBoundingVolume(type);
        bool overlaps;
        if (other) {
            overlaps = volume.TestCollision(*other, collision, false) != 1;
        } else {
            CVector3 position;
            node->GetPosition(position);
            overlaps = volume.TestCollision(position, collision, false) != 1;
        }
        if (overlaps && visitor.Visit(*node))
            return true;
    }
    return false;
}
