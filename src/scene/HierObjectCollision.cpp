// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"

int CHierObject::SetHierCollisionState(int index) {
    if (index == treeIndex) {
        hierFlags.collided = 1;
        return 1;
    }
    for (CHierObject *child = firstChild; child; child = child->nextSibling) {
        if (child->SetHierCollisionState(index)) {
            hierFlags.collided = 1;
            return 1;
        }
    }
    return 0;
}
