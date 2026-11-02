// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"

IVolume *CHierObject::GetSubVolume(int index) const {
    if (hierFlags.root && index > 0) {
        CHierObject *object = subObjects[index];
        if (object)
            return object->GetSubVolume(index);
        return 0;
    }
    return (IVolume *)field_13c;
}

IVolume *CHierObject::GetWorldBoundingVolume(ISceneNode::EVolumeType type) const {
    switch (type) {
    case 3:
        if (hierFlags.root)
            return (IVolume *)csgVolume;
        return (IVolume *)field_13c;
    case 0:
    case 1:
    case 2:
        if (hierFlags.root)
            return (IVolume *)csgVolume;
        return (IVolume *)field_13c;
    case 4:
        return 0;
    }
    return 0;
}

void CHierObject::SetInheritedMatrix(CMatrix &matrix) { inheritedMatrix = matrix; }
void CHierObject::SetAttachPointMatrix(CMatrix &matrix) { attachPointMatrix = matrix; }
int CHierObject::IsVisible(CDrawContext &) const { return 1; }

void CHierObject::Draw(CDrawContext &context) {
    CStaticObject::Draw(context);
    for (CHierObject *child = firstChild; child; child = child->nextSibling)
        ((ISceneNode *)child)->Draw(context);
}
