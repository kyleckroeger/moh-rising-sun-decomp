// CScene: applying queued changes to the player, world, camera and sky box slots and to the node lists.
// Link offsets are established by these traversals; member names are descriptive.
#include "SceneNode.h"

// Member-only view of a node's list links, established by these traversals.
struct SceneNodeLinks {
    unsigned char unknown_00[0x18];
    SceneNodeLinks *prev;
    SceneNodeLinks *next;
};

static inline void LinkNode(ISceneNode *&first, ISceneNode *&last, int &count, ISceneNode *nodeArg, ISceneNode *afterArg) {
    SceneNodeLinks *node = (SceneNodeLinks *)nodeArg;
    SceneNodeLinks *after = (SceneNodeLinks *)afterArg;
    // The original updates the tail on both paths, even for an insertion in mid-list.
    if (after) {
        SceneNodeLinks *next = after->next;
        after->next = node;
        next->prev = node;
        node->prev = after;
        node->next = next;
    } else {
        if (last)
            ((SceneNodeLinks *)last)->next = node;
        node->prev = (SceneNodeLinks *)last;
        if (!first)
            first = nodeArg;
    }
    last = nodeArg;
    count++;
}

// The object lists link through other offsets.
struct TypedObjectLinks {
    unsigned char unknown_00[0x44];
    ISceneNode *prev;
    ISceneNode *next;
};
struct LightLinks {
    unsigned char unknown_00[0x38];
    ISceneNode *prev;
    ISceneNode *next;
};

void CScene::AddPlayer_now(SModCommand &command) {
    ISceneNode *player = (ISceneNode *)command.arg0;
    ISceneNode *after = (ISceneNode *)command.arg1;
    for (int i = 0; i < playerCount; i++) {
        if (player == (ISceneNode *)players[i])
            return;
    }
    players[playerCount++] = (CPlayerObject *)player;
    LinkNode(firstNode, lastNode, nodeCount, player, after);
    AddNodeExtents(*player);
}

void CScene::SetWorld_now(SModCommand &command) {
    ISceneNode *node = (ISceneNode *)command.arg0;
    ISceneNode *after = (ISceneNode *)command.arg1;
    if (node != (ISceneNode *)world) {
        if (world) {
            SceneNodeLinks *old = (SceneNodeLinks *)world;
            SceneNodeLinks *prev = old->prev;
            SceneNodeLinks *next = old->next;
            if (firstNode == (ISceneNode *)old)
                firstNode = (ISceneNode *)next;
            if (lastNode == (ISceneNode *)old)
                lastNode = (ISceneNode *)prev;
            if (prev)
                prev->next = next;
            if (next)
                next->prev = prev;
            old->prev = 0;
            old->next = 0;
            nodeCount--;
            RemoveNodeExtents(*(ISceneNode *)world);
        }
        if (node) {
            LinkNode(firstNode, lastNode, nodeCount, node, after);
            AddNodeExtents(*node);
        }
        world = (CWorldObject *)node;
    }
}

void CScene::SetCamera_now(SModCommand &command) {
    camera = (CCamera *)command.arg0;
}

void CScene::SetSkyBox_now(SModCommand &command) {
    CSkyBox *newSkyBox = (CSkyBox *)command.arg0;
    if (newSkyBox != skyBox)
        skyBox = newSkyBox;
}

void CScene::AddNode_now(SModCommand &command) {
    ISceneNode *node = (ISceneNode *)command.arg0;
    LinkNode(firstNode, lastNode, nodeCount, node, (ISceneNode *)command.arg1);
    CAnimObject *anim = node->AsAnimObject();
    CStaticObject *object = node->AsStaticObject();
    CLight *light = node->AsLight();
    CProjector *projector = node->AsProjector();
    if (anim) {
        ISceneNode *n = (ISceneNode *)anim;
        if (lastAnimObject)
            ((TypedObjectLinks *)lastAnimObject)->next = n;
        ((TypedObjectLinks *)n)->prev = lastAnimObject;
        if (!firstAnimObject)
            firstAnimObject = n;
        lastAnimObject = n;
        animObjectCount++;
    } else if (object) {
        ISceneNode *n = (ISceneNode *)object;
        if (lastStaticObject)
            ((TypedObjectLinks *)lastStaticObject)->next = n;
        ((TypedObjectLinks *)n)->prev = lastStaticObject;
        if (!firstStaticObject)
            firstStaticObject = n;
        lastStaticObject = n;
        staticObjectCount++;
    } else if (light) {
        ISceneNode *n = (ISceneNode *)light;
        if (lastLight)
            ((LightLinks *)lastLight)->next = n;
        ((LightLinks *)n)->prev = lastLight;
        if (!firstLight)
            firstLight = n;
        lastLight = n;
        lightCount++;
    } else if (projector) {
        ISceneNode *n = (ISceneNode *)projector;
        if (lastProjector)
            ((LightLinks *)lastProjector)->next = n;
        ((LightLinks *)n)->prev = lastProjector;
        if (!firstProjector)
            firstProjector = n;
        lastProjector = n;
        projectorCount++;
    }
    if (!light)
        AddNodeExtents(*node);
}

void CScene::RemoveNode_now(SModCommand &command) {
    ISceneNode *node = (ISceneNode *)command.arg0;
    if (IsNodeInScene(node)) {
        SceneNodeLinks *links = (SceneNodeLinks *)node;
        SceneNodeLinks *prev = links->prev;
        SceneNodeLinks *next = links->next;
        if (firstNode == node)
            firstNode = (ISceneNode *)next;
        if (lastNode == node)
            lastNode = (ISceneNode *)prev;
        if (prev)
            prev->next = next;
        if (next)
            next->prev = prev;
        links->prev = 0;
        links->next = 0;
        nodeCount--;
        CStaticObject *object = node->AsStaticObject();
        CAnimObject *anim = node->AsAnimObject();
        CLight *light = node->AsLight();
        CProjector *projector = node->AsProjector();
        if (anim) {
            TypedObjectLinks *o = (TypedObjectLinks *)anim;
            ISceneNode *p = o->prev;
            ISceneNode *n = o->next;
            if (firstAnimObject == (ISceneNode *)anim)
                firstAnimObject = n;
            if (lastAnimObject == (ISceneNode *)anim)
                lastAnimObject = p;
            if (p)
                ((TypedObjectLinks *)p)->next = n;
            if (n)
                ((TypedObjectLinks *)n)->prev = p;
            o->prev = 0;
            o->next = 0;
            animObjectCount--;
        } else if (object) {
            TypedObjectLinks *o = (TypedObjectLinks *)object;
            ISceneNode *p = o->prev;
            ISceneNode *n = o->next;
            if (firstStaticObject == (ISceneNode *)object)
                firstStaticObject = n;
            if (lastStaticObject == (ISceneNode *)object)
                lastStaticObject = p;
            if (p)
                ((TypedObjectLinks *)p)->next = n;
            if (n)
                ((TypedObjectLinks *)n)->prev = p;
            o->prev = 0;
            o->next = 0;
            staticObjectCount--;
        } else if (light) {
            LightLinks *o = (LightLinks *)light;
            ISceneNode *p = o->prev;
            ISceneNode *n = o->next;
            if (firstLight == (ISceneNode *)light)
                firstLight = n;
            if (lastLight == (ISceneNode *)light)
                lastLight = p;
            if (p)
                ((LightLinks *)p)->next = n;
            if (n)
                ((LightLinks *)n)->prev = p;
            o->prev = 0;
            o->next = 0;
            lightCount--;
        } else if (projector) {
            LightLinks *o = (LightLinks *)projector;
            ISceneNode *p = o->prev;
            ISceneNode *n = o->next;
            if (firstProjector == (ISceneNode *)projector)
                firstProjector = n;
            if (lastProjector == (ISceneNode *)projector)
                lastProjector = p;
            if (p)
                ((LightLinks *)p)->next = n;
            if (n)
                ((LightLinks *)n)->prev = p;
            o->prev = 0;
            o->next = 0;
            projectorCount--;
        }
        if (!light)
            RemoveNodeExtents(*node);
        SExclusionEntry *it = exclusions.Begin();
        while (it != exclusions.End()) {
            if (it->low == node || it->high == node)
                exclusions.Erase(it);
            else
                it++;
        }
    }
}
