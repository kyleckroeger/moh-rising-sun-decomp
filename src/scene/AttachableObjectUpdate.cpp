// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original attachobject.cpp; member names are descriptive.
#include "SceneObjects.h"

void CAttachableObject::UpdateAttachedObjects() {
    for (int i = 0; i < 20; i++) {
        if (children[i])
            UpdateSingleAttachedObject(i);
    }
}
