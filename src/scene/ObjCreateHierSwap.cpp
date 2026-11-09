// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original objcreate.cpp.
#include "SceneObjects.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "ObjCreate.h"

class CStaticObjectFactory {
  public:
    static CStaticMesh *GetStaticMesh(int, int);
    static MotionData *GetMotionData(int, int);
};

void SwapHierObject(CHierObject *object, int sub, int index) {
    int type = ((BSObject *)((ISceneNode *)object)->GetScriptObject())->nativeObject->GetChildType();
    CStaticMesh *mesh = CStaticObjectFactory::GetStaticMesh(type, index);
    MotionData *motion = CStaticObjectFactory::GetMotionData(type, index);
    ((CHierObject *)object->GetSubObject(sub))->SwapOut(mesh, motion);
}

void DestroySubHierObject(CHierObject *object, int sub) {
    ((CStaticObjectInterface *)object->GetSubObject(sub))->CleanUp();
}
