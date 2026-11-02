// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "ObserverTypes.h"

void CHierAnimObject::Init(CHierObject *parent, int a, int b, int c, CVector3 &position, CAnimObject *object) {
    animObject = object;
    AttachAnimatedObject();
    CHierObject::Init(parent, a, b, c, position);
}

void CHierAnimObject::CleanUp() {
    ((ISubject *)animObject)->MarkForDestruction(1);
    CHierObject::CleanUp();
}

void CHierAnimObject::Draw(CDrawContext &context) {
    ((ISceneNode *)animObject)->Draw(context);
    for (CHierObject *child = firstChild; child; child = child->nextSibling)
        ((ISceneNode *)child)->Draw(context);
}

IVolume *CHierAnimObject::GetWorldBoundingVolume(ISceneNode::EVolumeType type) const {
    return (IVolume *)((ISceneNode *)animObject)->GetWorldBoundingVolume(type);
}
