// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original attachobject.cpp; member names are descriptive.
#include "SceneObjects.h"

int CAttachableObject::GetFreeAttachSlot() {
    ISceneNode **slot = children;
    for (int i = 0; i < 20; i++) {
        if (!*slot++)
            return i;
    }
    return -1;
}
int CAttachableObject::GetFirstChildHandle() {
    ISceneNode **slot = children;
    for (int i = 0; i < 20; i++) {
        if (*slot++)
            return i;
    }
    return -1;
}
int CAttachableObject::GetNextChildHandle(int handle) {
    if ((unsigned int)handle > 19)
        return -1;
    for (int i = handle + 1; i <= 19; i++) {
        if (children[i])
            return i;
    }
    return -1;
}
ISceneNode *CAttachableObject::GetChild(int handle) {
    if ((unsigned int)handle > 19)
        return 0;
    return children[handle];
}
