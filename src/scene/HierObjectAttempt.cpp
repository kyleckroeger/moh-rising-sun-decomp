// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original hierobject.cpp; member names are descriptive.
#include "SceneObjects.h"

void CHierObject::AttemptUpdate(float dt) {
    AttemptUpdateFromMotion(dt);
    for (CHierObject *child = firstChild; child; child = child->nextSibling) {
        child->SetInheritedMatrix(transform);
        ((ISceneNode *)child)->AttemptUpdate(dt);
    }
    AttemptUpdateOfAttachedObjects();
}

