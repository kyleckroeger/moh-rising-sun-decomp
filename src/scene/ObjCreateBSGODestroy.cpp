// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp.
#include "SceneObjects.h"
#include "ScriptPool.h"
#include "ScriptTimers.h"
#include "ObjCreate.h"

class CAnimObjectFactory {
  public:
    void ReleaseObject(CAnimObject *);
};
extern CAnimObjectFactory g_AnimObjFactory;
void DestroyBSObject(BSObject *);
void FreeBoxVolume(void *);
class CStaticObjectFactory {
  public:
    static void DeallocateStaticObject(CStaticObject *);
};
class CPlayerObject {
  public:
    void CleanUp();
};

static inline void Unlink(BSGOBindingLink &link) {
    if (link.previous)
        link.previous->next = link.next;
    if (link.next)
        link.next->previous = link.previous;
    link.next = 0;
    link.previous = 0;
}

void *BSGO_Destructable::operator new(unsigned int) { return g_pObjectAllocators[0].GetFreeElement(1); }
void *BSGO_Soldier::operator new(unsigned int) { return g_pObjectAllocators[1].GetFreeElement(1); }
void *BSGO_Tank::operator new(unsigned int) { return g_pObjectAllocators[1].GetFreeElement(1); }
void *BSGO_Anim::operator new(unsigned int) { return g_pObjectAllocators[1].GetFreeElement(1); }
void BSGO_Destructable::operator delete(void *p) { g_pObjectAllocators[0].FreeElement(p); }
void BSGO_Soldier::operator delete(void *p) { g_pObjectAllocators[1].FreeElement(p); }
void BSGO_Tank::operator delete(void *p) { g_pObjectAllocators[1].FreeElement(p); }
void BSGO_Anim::operator delete(void *p) { g_pObjectAllocators[1].FreeElement(p); }

void BSGO_Soldier::Destroy() {
    g_numActiveSoldiers--;
    BSGOBinding *b = &binding;
    RemoveObjectFromList(this, (BSGO_Basic **)&g_pGermanList);
    g_scene.Remove(*b->sceneNode);
    ((CAnimObjectInterface *)b->sceneNode)->CleanUp();
    g_AnimObjFactory.ReleaseObject((CAnimObject *)b->sceneNode);
    DestroyBSObject(script);
    script = 0;
    Unlink(binding.link);
    b->sceneNode = 0;
    delete this;
}

void BSGO_Anim::Destroy() {
    RemoveObjectFromList(this, (BSGO_Basic **)&g_pAnimatedMechList);
    BSGOBinding *b = &binding;
    g_scene.Remove(*b->sceneNode);
    ((CAnimObjectInterface *)b->sceneNode)->CleanUp();
    g_AnimObjFactory.ReleaseObject((CAnimObject *)b->sceneNode);
    DestroyBSObject(script);
    script = 0;
    Unlink(binding.link);
    b->sceneNode = 0;
    delete this;
}

void BSGO_Tank::Destroy() {
    RemoveObjectFromList(this, (BSGO_Basic **)&g_pTankList);
    BSGOBinding *b = &binding;
    g_scene.Remove(*b->sceneNode);
    ((CStaticObjectInterface *)b->sceneNode)->CleanUp();
    DestroyBSObject(script);
    script = 0;
    Unlink(binding.link);
    b->sceneNode = 0;
    delete this;
}

void BSGO_Destructable::Destroy() {
    RemoveObjectFromList(this, (BSGO_Basic **)&g_pBoxList);
    BSGOBinding *b = &binding;
    g_scene.Remove(*b->sceneNode);
    int local = b->sceneNode->GetLocalBoundingVolume((ISceneNode::EVolumeType)3);
    if (local)
        FreeBoxVolume((void *)local);
    int world = b->sceneNode->GetWorldBoundingVolume((ISceneNode::EVolumeType)3);
    if (world && world != local)
        FreeBoxVolume((void *)world);
    
    switch (script->nativeObject->GetNativeBindingType()) {
    default:
        b = &binding;
        break;
    case 2:
        {
            BSGOBinding *current = &binding;
            g_pEnvModMechAllocator.FreeElement(current->sceneNode);
            b = current;
        }
        break;
    case 1:
    case 4:
        {
            BSGOBinding *current = &binding;
            CStaticObjectFactory::DeallocateStaticObject((CStaticObject *)current->sceneNode);
            b = current;
        }
        break;
    }
    DestroyBSObject(script);
    script = 0;
    Unlink(binding.link);
    b->sceneNode = 0;
    delete this;
}

void BSGO_Player::Destroy() {
    ((CPlayerObject *)sceneNode)->CleanUp();
    DestroyBSObject(script);
    script = 0;
    sceneNode = 0;
}
