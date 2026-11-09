// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp.
#include "SceneObjects.h"
#include "SceneNode.h"

class PickupInfo;
enum EObjectClass { EObjectClassUnknown0 = 0 };
class CAnimObjectFactory {
  public:
    CAnimObject *AllocateObject(unsigned int, EObjectClass);
};
extern CAnimObjectFactory g_AnimObjFactory;
class CStaticObjectFactory {
  public:
    static CStaticObject *AllocateStaticObject(int, int, PickupInfo *);
};

void CreateSubHierAnimObject(CHierObject *parent, int sub, int index, unsigned int attachment, int, int type, int animation) {
    BSObject *script = (BSObject *)((ISceneNode *)parent)->GetScriptObject();
    CAnimObject *anim = g_AnimObjFactory.AllocateObject(animation, EObjectClassUnknown0);
    ISceneNode *node = (ISceneNode *)CStaticObjectFactory::AllocateStaticObject(type, sub, 0);
    CHierAnimObject *object = (CHierAnimObject *)node->AsHierObject();
    ((CStaticObjectInterface *)object)->SetScript(script);
    g_scene.Add(*(ISceneNode *)anim, (ISceneNode *)parent);
    CVector3 offset(0.0f, 0.0f, 0.0f);
    object->Init(parent, sub, index, attachment, offset, anim);
}
