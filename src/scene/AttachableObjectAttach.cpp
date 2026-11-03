// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original attachobject.cpp; member names are descriptive.
#include "SceneObjects.h"
#include "CameraShake.h"
#include "ScriptRuntime.h"

// Records a child in the slot's bookkeeping arrays; the type is 1 static, 2 animated, 3 player.
int CAttachableObject::AttachObject(CStaticObject *object, int node) {
    int result;
    if (object) {
        g_scene.ReOrder((ISceneNode *)this, (ISceneNode *)object);
        int slot = GetFreeAttachSlot();
        ((CStaticObjectInterface *)this)->FindAttachNode(node, &childMatrices[slot]);
        children[slot] = (ISceneNode *)object;
        childQueueIdentity[slot] = ((BSObject *)((ISceneNode *)object)->GetScriptObject())->queueIdentity;
        childTypes[slot] = 1;
        childNodes[slot] = node;
        g_scene.ExcludeCollision(*(ISceneNode *)this, *(ISceneNode *)object, -1);
        result = 1;
    } else
        result = 0;
    return result;
}

int CAttachableObject::AttachObject(CPlayerObject *player, int node, bool hide) {
    int result;
    if (player) {
        g_scene.ReOrder((ISceneNode *)this, (ISceneNode *)player);
        int slot = GetFreeAttachSlot();
        ((CStaticObjectInterface *)this)->FindAttachNode(node, &childMatrices[slot]);
        children[slot] = (ISceneNode *)player;
        childQueueIdentity[slot] = ((BSObject *)((ISceneNode *)player)->GetScriptObject())->queueIdentity;
        childTypes[slot] = 3;
        childNodes[slot] = node;
        player->pathBits.mmgBlocked = 1;
        player->field_d48 = ((ISceneNode *)this)->GetScriptObject();
        player->flags.pitchWhileMounted = !hide;
        player->SetHideWeapon(hide);
        g_scene.ExcludeCollision(*(ISceneNode *)this, *(ISceneNode *)player, -1);
        UpdateSingleAttachedObject(slot);
        result = 1;
    } else
        result = 0;
    return result;
}

int CAttachableObject::AttachObject(CAnimObject *object, int node) {
    if (!object)
        return 0;
    g_scene.ReOrder((ISceneNode *)this, (ISceneNode *)object);
    int slot = GetFreeAttachSlot();
    ((CStaticObjectInterface *)this)->FindAttachNode(node, &childMatrices[slot]);
    children[slot] = (ISceneNode *)object;
    childQueueIdentity[slot] = ((BSObject *)((ISceneNode *)object)->GetScriptObject())->queueIdentity;
    childTypes[slot] = 2;
    childNodes[slot] = node;
    object->SetAttached(this != 0);
    g_scene.ExcludeCollision(*(ISceneNode *)this, *(ISceneNode *)object, -1);
    return 1;
}

