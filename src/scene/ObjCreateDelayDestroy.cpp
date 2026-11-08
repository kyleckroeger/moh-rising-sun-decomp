// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp.
#include "SceneObjects.h"
#include "ScriptEvents.h"
#include "ScriptTimers.h"
#include "ScriptRuntime.h"
#include "TriggerObject.h"
#include "ObjCreate.h"

void AddObjectToDestroyList(BSGO_Basic *);
extern int BSSendMessage(unsigned short, BSObject *, TriggerObject *, BSObject *, void *);

void DelayDestroyObject(BSGO_Basic *object) {
    AddObjectToDestroyList(object);
    if (object->script) {
        object->script->queueIdentity = ~0U;
        if (object->script && object->script->nativeObject && object->script->nativeObject->scriptObject)
            BSObjectTriggerEvent(object->script->nativeObject->scriptObject, 380, 0, 0, true);
    }
}

void AddObjectToDestroyList(BSGO_Basic *object) {
    if (object->GetSceneNode())
        ((IDestructible *)object->GetSceneNode())->MarkForDestruction(2);
}

void DestroySingleQueueElement() {
    BSGO_Basic *object = g_pDelayDestroyList[g_delayDestroyHead].object;
    BSSendMessage(264, object->script, object->script->nativeObject, object->script, object->script);
    object->Destroy();
    g_pDelayDestroyList[g_delayDestroyHead].frame = 0;
    g_pDelayDestroyList[g_delayDestroyHead].object = 0;
    g_delayDestroyHead++;
    if (g_delayDestroyHead > 9)
        g_delayDestroyHead = 0;
}
