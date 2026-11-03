// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original attachobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "CameraShake.h"
#include "ScriptRuntime.h"

int CAttachableObject::DetachObject(ISceneNode *object) {
    int detached = 0;
    int zero = 0;
    for (int i = 0; i < 20; i++) {
        if (children[i] == object) {
            g_scene.IncludeCollision(*(ISceneNode *)this, *children[i]);
            detached = 1;
            if (childTypes[i] == 2) {
                children[i]->AsAnimObject()->SetAttached(false);
            } else if (childTypes[i] == 3) {
                children[i]->AsPlayerObject()->pathBits.mmgBlocked = zero;
                children[i]->AsPlayerObject()->field_d48 = zero;
                children[i]->AsPlayerObject()->flags.pitchWhileMounted = detached;
                children[i]->AsPlayerObject()->SetHideWeapon(false);
            }
            children[i] = (ISceneNode *)zero;
            childTypes[i] = zero;
            childNodes[i] = zero;
        }
    }
    return detached;
}

void CAttachableObject::DetachAllObjects() {
    int zero = 0;
    int cleared = 1;
    for (int i = 0; i < 20; i++) {
        ISceneNode *child = children[i];
        if (child) {
            g_scene.IncludeCollision(*(ISceneNode *)this, *child);
            if (childTypes[i] == 2) {
                child->AsAnimObject()->SetAttached(false);
            } else if (childTypes[i] == 3) {
                child->AsPlayerObject()->pathBits.mmgBlocked = zero;
                child->AsPlayerObject()->flags.pitchWhileMounted = cleared;
                child->AsPlayerObject()->field_d48 = zero;
                child->AsPlayerObject()->SetHideWeapon(false);
            }
            children[i] = (ISceneNode *)zero;
            childTypes[i] = zero;
            childNodes[i] = zero;
        }
    }
}
