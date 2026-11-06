// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Fragments of the original AIObject.cpp; member names are descriptive.
#include "AITargeting.h"

extern int g_iAIObjectCount;
extern CAIObject *g_paioAIObjectListStart;

void CAIObject::AddToGlobalList() {
    nextGlobal = g_paioAIObjectListStart;
    g_paioAIObjectListStart = this;
    g_iAIObjectCount++;
}

void CAIObject::RemoveFromGlobalList() {
    CAIObject *previous = 0;
    CAIObject *object = g_paioAIObjectListStart;
    g_iAIObjectCount--;
    for (; object; object = object->nextGlobal) {
        if (object == this)
            break;
        previous = object;
    }
    if (previous)
        previous->nextGlobal = nextGlobal;
    else
        g_paioAIObjectListStart = nextGlobal;
    for (object = g_paioAIObjectListStart; object; object = object->nextGlobal)
        object->RemovePointersTo(this);
}
